-- To make a more informed decision, change the previous query to show both the country and the company's name.
with report as (
  select country, company_name, count(distinct order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
  from orders
  natural join order_details
  natural join customers
  group by country, company_name
  order by total
)
select country, company_name, total/order_cnt as avg
from report
where country in ('Venezuela', 'Mexico', 'Brazil', 'Argentina')
order by avg desc