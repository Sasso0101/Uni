-- Now list any customer that has not placed an order yet, without using subqueries, aggregation, or null checks. Return both the customer ID and the company name
select C.customer_id, company_name
from customers C
left join orders O on C.customer_id = O.customer_id
except
select C.customer_id, company_name
from customers C
join orders O on C.customer_id = O.customer_id