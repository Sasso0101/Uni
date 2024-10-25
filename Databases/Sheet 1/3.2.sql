-- With this information, the company can now decide which countries to target. Using this helper expression to filter only developing countries:
-- WHERE Country in ('Venezuela', 'Mexico', 'Brazil', 'Argentina')
-- list the countries and the average amount of money spent in each country
with report as (
  select country, count(distinct order_id) as order_cnt, sum(unit_price*quantity*(1-discount)) as total
  from orders
  natural join order_details
  natural join customers
  group by country
  order by total
)
select country, total/order_cnt as avg
from report
where country in ('Venezuela', 'Mexico', 'Brazil', 'Argentina')
order by avg desc