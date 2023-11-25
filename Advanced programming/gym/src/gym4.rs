fn find_equal<'a, 'b>(s1: &'a str, s2: &'b str) -> Option<(&'a str, &'b str)> {
    for i in 0..=(s1.len() - 2) {
        for j in 0..=(s2.len() - 2) {
            if s1[i..=i + 1] == s2[j..=j + 1] {
                return Some((&s1[i..=i + 1], &s2[j..=j + 1]));
            }
        }
    }
    None
}

use std::{collections::{LinkedList, HashMap}, ops::{Add, Sub}, fmt::Display, f32::consts::PI};

use rand::{distributions::Uniform, Rng};
fn lucky_slice<'a>(input_str: &'a str) -> Option<&str> {
    let between = Uniform::from('a'..='z');
    let mut rng = rand::thread_rng();
    let mut new_string: String = String::new();
    for _ in 0..input_str.len() {
        new_string.push(rng.sample(between));
    }
    println!("{}", new_string);
    return find_equal(input_str, &new_string).and_then(|(s1, _)| return Some(s1));
}

#[derive(Debug)]
struct Person<'a> {
    name: String,
    father: Option<&'a Person<'a>>,
    mother: Option<&'a Person<'a>>,
}

impl<'a> Person<'a> {
    fn new(name: String, father: Option<&'a Person>, mother: Option<&'a Person>) -> Person<'a> {
        Person {
            name,
            father,
            mother
        }
    }

    fn find_relatives(&self, generations: u32) -> Vec<&Person<'a>> {
        let mut relatives: Vec<&Person<'a>> = Vec::new();
        relatives.push(self);
        if generations != 0 {
            if let Some(father) = self.father {
                relatives.extend(father.find_relatives(generations - 1))
            }
            if let Some(mother) = self.mother {
                relatives.extend(mother.find_relatives(generations - 1))
            }
        }
        relatives
    }

    fn find_roots(&self) -> Vec<&Person<'a>> {
        let mut orphans: Vec<&Person<'a>> = Vec::new();
        if let Some(father) = self.father {
            orphans.extend(father.find_roots());
        }
        if let Some(mother) = self.mother {
            orphans.extend(mother.find_roots());
        }
        if (self.father).is_none() || (self.mother).is_none() {
            orphans.push(self)
        }
        orphans
    }
}

struct ImportantExcerpt<'a> {
    part: &'a str,
}
impl<'a, 'b> ImportantExcerpt<'b> {
    fn announce_and_return_part(&'a self, announcement: &'b str) -> &'b str {
        println!("Attention please: {}", announcement);
        self.part
    }
}

struct DoubleRef<'a, 'b, T> {
    r: &'a T,
    s: &'b T
}

struct DoubleRef1<'a, 'b: 'a, T> {
    r: &'a T,
    s: &'b T
}

trait Split<'a> {
    type ReturnType;

    fn split(&'a self) -> (Self::ReturnType, Self::ReturnType);
}

impl<'a> Split<'a> for String {
    type ReturnType = &'a str;
    fn split(&'a self) -> (Self::ReturnType, Self::ReturnType) {
        self.split_at(self.len()/2)
    }
}

impl<'a> Split<'a> for &[i32] {
    type ReturnType = &'a [i32];
    fn split(&'a self) -> (Self::ReturnType, Self::ReturnType) {
        self.split_at(self.len()/2)
    }
}

impl<'a> Split<'a> for LinkedList<f64> {
    type ReturnType = Self;
    fn split(&'a self) -> (Self::ReturnType, Self::ReturnType) {
        let mut left_part = self.clone();
        let right_part = left_part.split_off(self.len()/2);
        (left_part, right_part)
    }
}

#[derive(Default, Debug, Clone, Copy)]
struct Point {
    x: i32,
    y: i32
}

impl Add for Point {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}
impl Sub for Point {
    type Output = Self;

    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

#[derive(Debug)]
struct Circle {
    radius: u32,
    center: Point
}

#[derive(Debug)]
struct Rectangle {
    top_left: Point,
    bottom_right: Point
}

impl Default for Circle {
    fn default() -> Self {
        Self {
            radius: 1,
            center: Default::default()
        }
    }
}

impl Default for Rectangle {
    fn default() -> Self {
        Self {
            top_left: Point { x: -1, y: 1 },
            bottom_right: Point { x: 1, y: -1 },
        }
    }
}

#[derive(Default, Clone, Copy)]
struct Area {
    area: f32
}

impl Display for Area {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Area is {} cm2", self.area)
    }
}
impl Add for Area {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Area { area: self.area + rhs.area }
    }
}
fn sum_area(items: &[&dyn GetArea]) -> Area {
    let mut total = Area::default();

    items.iter().for_each(|f| {
        total = total + f.get_area()
    });
    total
}
trait GetArea {
    fn get_area(&self) -> Area;
}

impl GetArea for Point {
    fn get_area(&self) -> Area {
        Area {
            area: 0.0
        }
    }
}
impl GetArea for Circle {
    fn get_area(&self) -> Area {
        Area {
            area: self.radius.pow(2) as f32 *PI
        }
    }
}
impl GetArea for Rectangle {
    fn get_area(&self) -> Area {
        let diff = self.top_left - self.bottom_right;
        Area {
            area: (diff.x*diff.y) as f32
        }
    }
}
fn skip_prefix<'a>(telephone_number: &'a str, prefix: &str) -> &'a str {
    if telephone_number[0..prefix.len()] == *prefix {
        &telephone_number[prefix.len()..]
    } else {
        telephone_number
    }
}

struct Chair<'a> {
    color: &'a str,
    quantity: &'a str
}

impl Object for Chair<'_> {
    fn build(&self) -> &str {
        "Chair has been built"
    }

    fn get_quantity(&self) -> String {
        format!("There are {} chairs", self.quantity)
    }
}

impl Display for Chair<'_> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.quantity.parse::<usize>().unwrap() == 0 {
            write!(f, "No chairs!")
        } else if self.quantity.parse::<usize>().unwrap() == 1 {
            write!(f, "One chair!")
        } else {
            write!(f, "{} chairs!", self.quantity)
        }
    }
}

trait Object {
    fn build(&self) -> &str;
    fn get_quantity(&self) -> String;
}

#[derive(PartialEq, Eq)]
enum Role {
    GUEST,
    USER,
    ADMIN
}

#[derive(PartialEq, Eq, Hash)]
enum Permission {
    READ,
    WRITE,
    EXECUTE
}

struct Actions {
    action: String,
    permission: HashMap<Permission, bool>
}

struct User {
    name: String,
    role: Role,
    actions: Vec<Actions>
}

trait Auth {
    fn check_permission(&self, action: &str, permission_type: &Permission) -> bool;
    fn can_write(&self, action: &str) -> bool;
    fn can_read(&self, action: &str) -> bool;
    fn can_execute(&self, action: &str) -> bool;
}

impl Auth for User {
    fn check_permission(&self, action: &str, permission_type: &Permission) -> bool {
        match self.actions.iter().find(|el| {
            el.action == action
        }) {
            Some(action) => {
                *action.permission.get(permission_type).unwrap()
            },
            None => {
                false
            }
        }
    }

    fn can_write(&self, action: &str) -> bool {
        self.check_permission(action, &Permission::WRITE)
    }
    fn can_read(&self, action: &str) -> bool {
        self.check_permission(action, &Permission::READ)
    }
    fn can_execute(&self, action: &str) -> bool {
        self.check_permission(action, &Permission::EXECUTE)
    }
}

impl Default for Actions {
    fn default() -> Self {
        Self {
            action: "".to_string(),
            permission: HashMap::from([
                (Permission::READ, false),
                (Permission::WRITE, false),
                (Permission::EXECUTE, false),
            ])
        }
    }
}

impl Actions {
    fn new(action: String, read: bool, write: bool, execute: bool) -> Self {
        Actions { 
            action: action,
            permission: HashMap::from([
                (Permission::READ, read),
                (Permission::WRITE, write),
                (Permission::EXECUTE, execute),
            ])
        }
    }
}

impl Default for User {
    fn default() -> Self {
        Self { name: "Guest".to_string(), role: Role::GUEST, actions: Vec::new() }
    }
}

impl User {
    fn change_role(&mut self, role: Role) -> Result<(), String> {
        if 
            self.role == Role::ADMIN || 
            (self.role == Role::USER && (role == Role::GUEST || role == Role::USER)) ||
            (self.role == Role::GUEST && role == Role::USER) {
            self.role = role;
            Ok(())
        } else {
            Err("Permissions not sufficient!".to_string())
        }
    }
    fn sudo_change_permission(mut user: User, string: String, permission: Permission, value: bool) {
        if let Some(action) = user.actions.iter_mut().find(|el| {
            el.action == string
        }) {
            let permission = action.permission.get_mut(&permission).unwrap();
            *permission = value;
        };
    }
}
#[cfg(test)]
mod tests {
    use crate::gym4::*;

    #[test]
    fn exercise1() {
        let s = lucky_slice("helloimahuman");
        println!("{:?}", s);
    }

    #[test]
    fn exercise2() {
        let g1p1 = Person::new("G1p1".to_string(), None, None);
        let g2p1 = Person::new("G2p1".to_string(), None, None);
        let _g1p2 = Person::new("G1p2".to_string(), None, None);
        let g2p2 = Person::new("G2p2".to_string(), None, None);
        let p1 = Person::new("p1".to_string(), Some(&g1p1), None);
        let p2 = Person::new("p2".to_string(), Some(&g2p1), Some(&g2p2));
        let child = Person::new("child".to_string(), Some(&p1), Some(&p2));

        println!("{:#?}", child.find_relatives(2));
        println!("{:#?}", child.find_roots());
    }

    #[test]
    fn exercise6() {
        println!("{:?}", Point::default());
        println!("{:?}", Circle::default());
        println!("{:?}", Rectangle::default());
        println!("{:?}", Point {x: 10, y: 11} + Point{x: 1, y: 1});
        println!("{:?}", Point::default() - Point{x: 20, y: 30});
    }

    #[test]
    fn exercise7() {
        println!("{:?}", skip_prefix("+3912345678", "+39"));
        println!("{:?}", skip_prefix("+4012345678", "+39"));
    }
}
