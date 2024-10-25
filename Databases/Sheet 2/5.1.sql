-- List all the customers of Northwind, along with any order that may have been done by them
select C.customer_id, company_name
from customers C
left join orders O on C.customer_id = O.customer_id