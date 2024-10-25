-- Recycle Exercise 3.3’s Report CTE to create a view that lists the countries in which Northwind has customers. Call the view CountriesWithCustomers
drop view CountriesWithCustomers;
create view CountriesWithCustomers as (
  select country, company_name, count(distinct O.order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
  from orders O
  join order_details OD on OD.order_id = O.order_id
  join customers C on O.customer_id = C.customer_id
  group by country, company_name
  order by total
);