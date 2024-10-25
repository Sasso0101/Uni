-- Northwind Traders would like to investigate why these customers have not placed an order yet. To address this, NT would like potential customers and suppliers to contact each other
-- List side-by-side any customer that has not placed an order yet with potential suppliers that are located in the same country
-- Do not use subqueries, aggregation, or null checks. Print only the representatives’ names
select C.contact_name as "Customer", S.contact_name as "Supplier"
from (
  select contact_name, country
  from customers C
  left join orders O on C.customer_id = O.customer_id
  except
  select contact_name, country
  from customers C
  join orders O on C.customer_id = O.customer_id
) C
join suppliers S on C.country = S.country