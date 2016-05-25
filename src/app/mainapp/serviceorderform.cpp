#include "serviceorderform.h"
#include "ui_serviceorderform.h"

ServiceOrderForm::ServiceOrderForm( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::ServiceOrderForm )
{
    ui->setupUi( this );
    initFields();
    initErrors();

    ui->tableViewCustomers->setCustomerModel();
    ui->tableViewCustomers->setSelectionBehavior( QAbstractItemView::SelectRows );

    connectSlots();
}


void ServiceOrderForm::initFields()
{
    _db = Db::getInstance();
    _customerModel = QSharedPointer<CustomerModel>( new CustomerModel() );
    _addCustomerForm = QSharedPointer<AddCustomerForm>( new AddCustomerForm() );
    _addServiceForm = QSharedPointer<AddServiceForm>( new AddServiceForm() );
    _serviceModel = QSharedPointer<ServiceModel>( new ServiceModel() );
    _serviceOrderModel = QSharedPointer<ServiceOrderModel>(
                         new ServiceOrderModel() );
    _serviceOrderDetailModel = QSharedPointer<ServiceOrderDetailModel>(
                               new ServiceOrderDetailModel() );
}


void ServiceOrderForm::connectSlots()
{
    connect( ui->pushButtonAddCustomer
             , SIGNAL( clicked( bool ) )
             , _addCustomerForm.data()
             , SLOT( show() ) );

    connect( _addCustomerForm.data()
             , SIGNAL( updateCustomers() )
             , ui->tableViewCustomers
             , SLOT( setCustomerModel() ) );

    connect( ui->pushButtonFind
             , SIGNAL( clicked( bool ) )
             , SLOT( findCustomerByPhone() ) );

    connect( ui->pushButtonAddService
             , SIGNAL( clicked( bool ) )
             , _addServiceForm.data()
             , SLOT( show() ) );

    connect( _addServiceForm.data()
             , SIGNAL( addService( Service & ) )
             , SLOT( addService( Service & ) ) );

    connect( ui->pushButtonAddServiceOrder
             , SIGNAL( clicked( bool ) )
             , SLOT( addOrder() ) );
}


void ServiceOrderForm::findCustomerByPhone()
{
    QString customerPhone = ui->linePhone->text();

    if ( customerPhone.isEmpty() ) {
        ui->tableViewCustomers->setCustomerModel();
    }
    else {
        ui->tableViewCustomers->setCustomerModelByPhone( customerPhone );
    }
}


void ServiceOrderForm::addService( Service &service )
{
    _serviceList.append( service );
    addServiceToForm();
}


void ServiceOrderForm::addOrder()
{
    QString customerId =  ui->tableViewCustomers->getSelectedCustomerId();
    if ( customerId.isEmpty() ) {
        message( _errors[ Errors::NOT_CHOOSE_CUSTOMER ] );
        return;
    }

    if ( _serviceList.empty() ) {
        message( _errors[ Errors::PRODUCT_LIST_EMPTY ] );
        return;
    }

    bool transactionOk = _db->transaction();
    if ( !transactionOk ) {
        message( _errors[ Errors::ADD_ORDER_ERROR ] );
        return;
    }

    QString orderId = _serviceOrderModel->addOrder( customerId );
    if ( orderId.toInt() <= 0 ) {
        _db->rollback();
        message( _errors[ Errors::ADD_ORDER_ERROR ] );
        return;
    }

    QString serviceId = "-1";
    bool addOrderDetailOk = false;
    for ( Service &service : _serviceList ) {
        serviceId = _serviceModel->addService( service );
        if ( serviceId.toInt() <= 0  ) {
            _db->rollback();
             message( _errors[ Errors::ADD_ORDER_ERROR ] );
            return;
        }
        service.serviceId = serviceId;

        addOrderDetailOk = _serviceOrderDetailModel->addOrderDetail( orderId
                                                                     , serviceId );
        if ( !addOrderDetailOk ) {
            _db->rollback();
             message( _errors[ Errors::ADD_ORDER_ERROR ] );
            return;
        }
    }

    bool commitTransactionOk = _db->commit();
    if ( commitTransactionOk ) {
        message( _errors[ Errors::NO_ERRORR ] );
    }
    else {
        _db->rollback();
        message( _errors[ Errors::ADD_ORDER_ERROR ] );
    }
}


void ServiceOrderForm::addServiceToForm()
{
    auto table = ui->tableViewServiceOrder;
    Service &service = _serviceList.last();

    QVector<QTableWidgetItem*> items;

    items.push_back( new QTableWidgetItem( service.category ) );
    items.push_back( new QTableWidgetItem( service.description ) );
    items.push_back( new QTableWidgetItem( service.cost ) );

    int row = table->rowCount();
    table->insertRow( row );

    for ( int i = 0; i < items.size(); ++i ) {
        table->setItem( row, i, items[ i ] );
    }
}


void ServiceOrderForm::message( QString text )
{
    QMessageBox msgBox;
    msgBox.setText( text );
    msgBox.exec();
}


void ServiceOrderForm::initErrors()
{
    _errors[ Errors::NO_ERRORR ] =
        QString( tr( "Замовлення успішно додано!" ) );

    _errors[ Errors::PRODUCT_LIST_EMPTY ] =
        QString( tr( "Немає послуг для збереження!" ) );

    _errors[ Errors::NOT_CHOOSE_CUSTOMER ] =
        QString( tr( "Не вибрано замовника!" ) );

    _errors[ Errors::ADD_ORDER_ERROR ] =
        QString( tr( "Помилка при додаванні замовлення!" ) );
}


ServiceOrderForm::~ServiceOrderForm()
{
    delete ui;
}
