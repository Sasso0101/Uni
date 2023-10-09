#![allow(dead_code)]
#![allow(unused)]

fn prev_char(c: char) -> char {
    if c.is_alphabetic() && c != 'A' && c != 'a' {
        ((c as u8) - 1) as char
    } else { c}
}

fn prev_str(s: &str) -> String  {
    s.chars().into_iter().map(|c| {
        prev_char(c)
    }).collect()
}

use std::mem::replace;

struct X {
    s: Option<String>,
    i: i32
}

impl X {
    fn new(s: &str, i: i32) -> Self {
        Self {
            s: Some(s.to_string()),
            i
        }
    }

    fn take_str(&mut self) -> Option<String> {
        replace(&mut self.s, None)
    }
}

use std::mem::swap;

struct NameSurname {
    name: String,
    surname: String
}

fn replace_surname(mut ns: NameSurname, mut s: String) -> String {
    let mut old = String::from("");
    swap(&mut ns.surname, &mut old);
    swap(&mut ns.surname, &mut s);
    old
}

use std::fmt::{Display, Formatter};

#[derive(Debug, Clone)]
struct Student {
    name: String,
    id: u32
}

impl Student {
    fn new(name: &str, id: u32) -> Self {
        Self {
            name: name.to_string(),
            id
        }
    }
}

impl Display for Student {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Name: {} Id: {}", self.name, self.id)
    }
}

struct University {
    name: String,
    students: Vec<Student>
}

impl University {
    fn new(name: &str, student: &[Student]) -> Self {
        Self {
            name: name.to_string(),
            students: student.to_vec()
        }
    }

    fn remove_student(&mut self, id: u32) -> Result<Student, &str> {
        let pos = self.students.iter().position(|s| {
            s.id == id
        });
        match pos {
            Some(pos) => {
                Ok(self.students.remove(pos))
            },
            None => {
                Err("Not found")
            }
        }
    }
}

impl Display for University {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}\nStudents: {:?}", self.name, self.students)
    }
}


#[derive(PartialEq, Copy, Clone, Debug)]
enum AirplaneCompany {
    Airbus,
    Boeing
}
#[derive(Debug)]
struct Airplane {
    company: AirplaneCompany,
    model: String
}

#[derive(Debug)]
struct AirFleet {
    fleet: Vec<Airplane>
}

impl AirFleet {
    fn remove_boeing(&mut self) {
        self.fleet.retain(|airplane| {
            airplane.company != AirplaneCompany::Boeing
        })
    }

    fn add_airplane(&mut self, airplane: Airplane) {
        self.fleet.push(airplane)
    }

    fn search_airplane(&self, model: &str) -> Result<AirplaneCompany, String> {
        let pos = self.fleet.iter().position(|airplane| {
            airplane.model == model
        });
        match pos {
            Some(pos) => {
                Ok(self.fleet[pos].company)
            },
            None => {
                Err("Not in this fleet".to_string())
            }
        }
    }
}


use std::collections::HashMap;
struct Maps {
    map: HashMap<usize, String>
}

fn string_to_tuple(hm: Maps) -> HashMap<usize, (usize, String)> {
    hm.map.into_iter().map(|(us, s)| {
        (us, (us, s))
    }).collect()
}

struct Size {
    width: f32,
    height: f32
}

impl Size {
    fn new(width: f32, height: f32) -> Self {
        Self {
            width,
            height
        }
    }

    fn area(&self) -> f32 {
        self.width * self.height
    }

    fn compare(&self, s: &Size) -> Option<bool> {
        if self.area() == s.area() {None}
        else { Some(self.area() > s.area()) }
    }
}

struct MaybePoint {
    x: Option<i32>,
    y: Option<i32>,
}

impl MaybePoint {
    fn new(x: Option<i32>, y: Option<i32>) -> Self {
        Self {
            x, y
        }
    }

    fn is_some(&self) -> bool {
        self.x.is_some() && self.y.is_some()
    }

    fn maybe_len(&self) -> Option<f32> {
        if self.x.is_none() || self.y.is_none() {None}
        else {
            Some(((self.x.unwrap() as f32).powi(2)+(self.y.unwrap() as f32).powi(2)).sqrt())
        }
    }
}

fn res1(n: i32) -> Result<i32, String> {
    if n % 10 == 0 { Ok(n) }
    else { Err("error".to_string()) }
}

fn res2(n: Result<i32, &str>) -> Result<i32, String> {
    let n = n?;
    if n % 5 == 0 {Ok(n)}
    else {Err("error".to_string())}
}

fn wrapper(n: i32) -> Result<i32, String> {
    let n = res1(n)?;
    res2(Ok(n))
}


fn order(v: Vec<String>) -> Vec<String> {
    v.iter().enumerate().map(|(i, s)| {
       format!("{} - {}", i, s)
    }).collect()
}


mod modx {
    pub enum X {
        S(char),
        C(String)
    }
}

mod mody {
    pub enum X {
        F(f64, usize)
    }
}

mod modsum {
    use super::modx::X;
    use super::mody::X as Y;

    pub fn sum(e1: X, e2: Y) -> u8 {
        let n1 = match e1 {
            X::S(c) => {c as i32}
            X::C(s) => {s.len() as i32}
        };
        match e2 { Y::F(i, u) => {(n1 as f64+(i*u as f64)) as u8} }
    }
}

pub fn main() {
    let c = 's';
    println!("{}", prev_char(c));

    let s = "abcdefg_1234_ABCDEFG";
    let s = prev_str(s);
    println!("{}", s);


    let mut x = X::new("ciao", 10);

    let s = x.take_str();
    println!("{}", s.unwrap() );

    
    let person1: NameSurname = NameSurname {
        name: "Ernesto".to_string(),
        surname: "Bianchi".to_string(),
    };
    let surname = "sassi".to_string();
    let a = replace_surname(person1, surname);
    println!("{}", a);


    let s1 = Student::new("marco", 1);
    let s2 = Student::new("anto", 2);
    let s3 = Student::new("anna", 3);
    let mut university = University::new("Trento", &vec![s1, s2, s3]);

    println!("{}", university);

    println!("{}", university.remove_student(1).unwrap().id);


    let mut fleet = AirFleet{
        fleet: Vec::new(),
    };

    let airplane1 = Airplane{
        company: AirplaneCompany::Airbus,
        model: "A380".to_string(),
    };

    let airplane2 = Airplane{
        company: AirplaneCompany::Boeing,
        model: "747".to_string(),
    };

    let airplane3 = Airplane{
        company: AirplaneCompany::Airbus,
        model: "A320".to_string(),
    };

    fleet.add_airplane(airplane1);
    fleet.add_airplane(airplane2);
    fleet.add_airplane(airplane3);

    println!("{:?}", fleet.search_airplane("A380"));
    println!("{:?}", fleet.search_airplane("747"));
    println!("{:?}", fleet.search_airplane("A320"));
    println!("{:?}", fleet.search_airplane("A330"));


    let mut hashmap = HashMap::new();
    hashmap.insert(1, "ciao".to_string());
    hashmap.insert(2, "ciao".to_string());
    hashmap.insert(3, "ciao".to_string());

    let hashmap = Maps{
        map: hashmap,
    };

    let hashmap = string_to_tuple(hashmap);

    println!("{:?}", (hashmap.get(&1).unwrap().0, hashmap.get(&1).unwrap().1.clone()));


    use std::fmt::{Debug, Formatter};

    impl Debug for Size {
        fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
            write!(f, "Size {{ width: {}, height: {} }}", self.width, self.height)
        }
    }

    let s = Size::new(5.7, 1.2);

    println!("{:?}", s.area());
    println!("{:?}", s.compare(&Size::new(8.9, 10.)));
    println!("{:?}", s.compare(&Size::new(1.8, 0.1)));
    println!("{:?}", s.compare(&Size::new(5.7, 1.2)));


    let x = MaybePoint::new(Some(10),Some(20));
    let y = MaybePoint{x:Some(10),y:None};

    println!("{:?}", x.is_some());
    println!("{:?}", y.is_some());
    println!("{:?}", x.maybe_len());
    println!("{:?}", y.maybe_len());


    println!("{:?}", wrapper(10));
    println!("{:?}", wrapper(5));
    println!("{:?}", wrapper(11));


    let a :Vec<String> = vec!["Ciao".to_string(), "Come".to_string(), "Va".to_string()];

    let b = order(a);
    println!("{}", b.get(1).unwrap().to_owned());
    println!("{}", b.get(2).unwrap().to_owned());

    use modsum::sum;

    println!("{}", sum(modx::X::S(' '), mody::X::F(1.2, 4)));
    println!("{}", sum(modx::X::C("hello".to_owned()), mody::X::F(2.4, 10)));
}