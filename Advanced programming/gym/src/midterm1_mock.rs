use std::fmt::{Display, format};

fn prev_str(s: &str) -> String {
    s.chars().map(|c| {
        if c != 'A' && c != 'a' {(( c as u8) - 1) as char}
       else {c}
    }).collect()
}

struct X {
    s: Option<String>,
    i: i32
}

impl X {
    fn new(s: &str, i: i32) -> Self {
        X {
            s: Some(s.to_string()),
            i
        }
    }

    fn take_str(&mut self) -> &Option<String> {
        self.s = None;
        &self.s
    }
}

struct NameSurname {
    name: String,
    surname: String
}

fn replace_surname(ns: &mut NameSurname, surname: String) -> String {
    let old_surname = ns.surname.clone();
    ns.surname = surname;
    old_surname
}

struct Student {
    name: String,
    id: u32
}

impl Student {
    fn new(name: &str, id: u32) -> Student {
        Student {
            name: name.to_string(),
            id
        }
    }
}

impl Display for Student {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Name: {}, ID: {}", self.name, self.id)
    }
}

struct University {
    name: String,
    students: Vec<Student>
}

impl University {
    fn new(name: &str, student: &[Student]) -> University {
        University { 
            name: name.to_string(),
            students: vec![Student::new((*student)[0].name.as_str(), (*student)[0].id)]
        }
    }
    
    fn remove_student(&mut self, id: u32) -> Result<Student, &str> {
        let pos = self.students.iter().position(|student| {
            student.id == id
        });
        match pos {
            Some(pos) => {
                let st = self.students.remove(pos);
                Ok(st)
            },
            None => {
                Err("Not found")
            }
        }
    }
}

impl Display for University {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let s = self.students.iter().fold(String::new(), |acc, s| {
            format!("{}\n{}", acc, s)
        });
        write!(f, "Name: {}, Students:\n{}", self.name, s)
    }
}

#[derive(PartialEq, Clone)]
enum AirplaneCompany {
    Airbus,
    Boing
}
struct Airplane {
    company: AirplaneCompany,
    model: String
}
struct AirFleet {
    airplanes: Vec<Airplane>
}

impl AirFleet {
    fn remove_boing(&mut self) {
        self.airplanes.retain(|el| {
            el.company != AirplaneCompany::Boing
        })
    }

    fn add_airplane(&mut self, plane: Airplane) {
        self.airplanes.push(plane)
    }

    fn search_airplane(&self, model: String) -> Result<AirplaneCompany, String> {
        match self.airplanes.iter().find(|el| {
            el.model == model
        }) {
            Some(el) => {
                Ok(el.company.clone())
            },
            None => {
                Err("No airplane found".to_string())
            }
        }
    }
}

mod unumber {
    #[derive(PartialEq, Eq, Hash)]
    pub struct Unumber(pub usize);
}

mod hashmaps {
    use std::collections::HashMap;

    use super::unumber;

    pub struct Maps {
        pub map: HashMap<unumber::Unumber, String>
    }
}

mod another_module {
    use std::collections::HashMap;

    use super::{hashmaps, unumber};

    fn string_to_tuple(m: hashmaps::Maps) -> HashMap<unumber::Unumber, (unumber::Unumber, String)> {
        m.map.into_iter().map(|el| {
            (el.0, (unumber::Unumber(el.1.len()), el.1))
        }).collect()
    }
}

struct Size {
    width: u32,
    size: u32
}

impl Size {
    fn new(width: u32, size: u32) -> Size {
        Size {
            width, size
        }
    }

    fn area(&self) -> u32 {
        self.width*self.size
    }

    fn compare(&self, s: Size) -> Option<bool> {
        if self.area() == s.area() {
            None
        } else  {
            Some(self.area() > s.area())
        }
    }
}

struct MaybePoint {
    x: Option<i32>,
    y: Option<i32>
}

impl MaybePoint {
    fn new(x: Option<i32>, y: Option<i32>) -> MaybePoint {
        MaybePoint { x, y }
    }

    fn is_some(&self) -> bool {
        self.x.is_some() && self.y.is_some()
    }

    fn maybe_len(&self) -> Option<f32> {
        if self.is_some() {
            Some(((self.x.unwrap() as f32).powi(2) + (self.y.unwrap() as f32).powi(2)).sqrt())
        } else { None }
    }
}

fn res1(n: i32) -> Result<i32, String> {
    if n % 10 == 0 {
        Ok(n)
    } else {
        Err("error".to_string())
    }
}

fn res2(n: i32) -> Result<i32, String> {
    if n % 5 == 0 {
        Ok(n)
    } else {
        Err("error".to_string())
    }
}

fn wrapper(n: i32) -> Result<i32, String> {
    res1(n)?;
    res2(n)?;
    Ok(n)
}

fn order(v: Vec<String>) -> Vec<String> {
    v.iter().enumerate().map(|(i, el)| {
        format!("{} - {}", i, el)
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
    use super::{modx, mody};

    fn sum(x: modx::X, y: mody::X) -> u8 {
        let n1 = match x {
            modx::X::S(c) => {
                c as u8
            },
            modx::X::C(s) => {
                s.len() as u8
            }
        };
        let n2 = match y {
            mody::X::F(f1, f2) => {
                f1 * f2 as f64
            }
        };
        n1 + n2 as u8
    }
}

pub fn main() {
    println!("{} becomes {}", "abcdeABCDE", prev_str("abcdeABCDE"));
}