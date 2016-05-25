#include "productorderview.h"

ProductOrderView::ProductOrderView( QWidget *parent )
    : QTableWidget( parent )
{ 
    setColumnCount(5);
    setHeaders();
    horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
}


void ProductOrderView::setHeaders()
{
    QStringList headers;
    headers << tr( "Назва товару" )
            << tr( "Штрих-код" )
            << tr( "Категорія" )
            << tr( "Кількіть" )
            << tr( "Вартість одиниці" );

    setHorizontalHeaderLabels( headers );
}
