-- List all the employees (Name, Surname, count of orders shipped) that
-- - were hired when they were younger than 35 years old
-- - are located in the UK
-- - have shipped at least 45 orders
select E.first_name, E.last_name, count(*)
from employees E
join orders O on E.employee_id = O.employee_id
where country = 'UK' and (hire_date-birth_date)/365 < 35
group by E.employee_id
having count(*) >= 45