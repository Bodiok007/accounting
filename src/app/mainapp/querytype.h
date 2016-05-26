#ifndef QUERYTYPE_H
#define QUERYTYPE_H

enum class QueryType
{
    LOGIN,
    ADD_EMPLOYEE,
    ADD_ROLE_TO_EMPLOYEE,
    GET_ROLES,
    REMOVE_ROLE_FROM_EMPLOYEE,
    UPDATE_USER,
    UPDATE_PASSWORD,
    ADD_PRODUCT,
    GET_PRODUCT_CATEGORIES,
    ADD_PRODUCT_ORDER,
    ADD_PRODUCT_ORDER_DETAIL,
    GET_PRODUCT,
    GET_PRODUCT_ORDER,
    GET_PRODUCT_BY_ORDER_ID,
    GET_CUSTOMER,
    ADD_CUSTOMER,
    GET_SERVICE_CATEGORIES,
    GET_CUSTOMER_BY_PHONE,
    ADD_SERVICE,
    ADD_SERVICE_ORDER_DETAIL,
    ADD_SERVICE_ORDER,
    GET_SERVICE_ORDER,
    GET_SERVICE_BY_ORDER_ID
};

#endif // QUERYTYPE_H
