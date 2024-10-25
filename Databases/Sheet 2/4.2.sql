-- The employees listed in the previous query have been selected for a special training program. Northwind Traders’ Megadirector – your boss – would like to know more about these employees. Provide a detailed report for him: in each row of the report, list
-- - the employee IDs as found in the previous query,
-- - a country the employee has shipped to,
-- - the average number of days it took for the employee to ship an order to that country
-- - how many distinct products the employee has shipped to that country
-- Make sure rows with incomplete data are stripped out.
select employee_id, ship_country, avg(shipped_date - order_date), count(distinct product_id)
from orders
natural join order_details
where employee_id in (
  select E.employee_id
  from employees E
  join orders O on E.employee_id = O.employee_id
  where country = 'UK' and (hire_date-birth_date)/365 < 35
  group by E.employee_id
  having count(*) >= 45
) and shipped_date is not null
group by employee_id, ship_country