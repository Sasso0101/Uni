-- Are you able to list all the companies located in developing countries that spend less than the average in their respective countries?
with company_avg as (
  select country, company_name, count(distinct order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
  from orders
  natural join order_details
  natural join customers
  group by country, company_name
),
country_avg as (
  select country, count(distinct order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
  from orders
  natural join order_details
  natural join customers
  group by country
)
select COMP.country, company_name, COMP.total/COMP.order_cnt as comp_avg, COUNT.total/COUNT.order_cnt as country_avg
from company_avg COMP
join country_avg COUNT on COMP.country = COUNT.country
where COMP.country in ('Venezuela', 'Mexico', 'Brazil', 'Argentina') and
COMP.total/COMP.order_cnt < COUNT.total/COUNT.order_cnt
order by comp_avg desc