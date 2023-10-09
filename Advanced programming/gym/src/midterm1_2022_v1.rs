#![allow(dead_code, unused, non_snake_case)]

use std::fmt;

enum XX {
    Y1(i32, i32),
    Y2(String)
}
fn data(s: &XX) -> i32 {
    match s {
        XX::Y1(n1, n2) => {
            n1 * n2
        },
        XX::Y2(y2) => {
            y2.len() as i32
        }
    }
}

enum Z {
    Y1(i32, i32),
    Y2(bool, String)
}

fn maybelength(s: &Z) -> Option<u32> {
    match s {
        Z::Y1(_,_) => {
            None
        },
        Z::Y2(b, st) => {
            Some(st.len() as u32)
        }
    }
}

mod enumx {
    pub enum X {
        Y(String)
    }
}

mod structx {
    pub struct X {
        pub i: String
    }
}

mod modfun {
    use super::{enumx, structx};
    pub fn longer(e1: &enumx::X, e2: &structx::X) -> usize {
        match e1 {
            enumx::X::Y(s) => {
                if s.len() > e2.i.len() {s.len()}
                else {e2.i.len()}
            }
        }
    }
}

fn maybesqrt(n: i32) -> Option<i32> {
    if n >= 0 {Some((n as f32).sqrt() as i32)}
    else {None}
}

struct Balance {
    amt: i32
}

impl Balance {
    pub fn maybewithdraw(&self, n: i32) -> Option<i32> {
        if self.amt >= n {
            Some(self.amt - n)
        } else {
            None
        }
    }
}

fn prevchar(c: char) -> char {
    if (c == 'a') {
        '\''
    } else {
        ((c as u8) - 1) as char
    }
}

fn replwithprev(s: &mut String) -> Result<String, ()> {
    if s.contains('a') {Err(())}
    else {
        Ok(s.chars().map(|c| {
            prevchar(c)
        }).collect())
    }
}

#[derive(Debug)]
struct X {
    s: String,
    i: i32
}

impl fmt::Display for X {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "S {}, I {}", self.s, self.i)
    }
}

impl X {
    fn new() -> Self {
        Self {
            s: "xxx".to_string(),
            i: 10
        }
    }
    fn getstring(&mut self) -> String {
        std::mem::replace(&mut self.s, "".to_string())
    }
}

#[derive(Debug)]
struct Y {
    b: bool,
    c: String
}
impl fmt::Display for Y {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "B {}, C {}", self.b, self.c)
    }
}

impl Y {
    fn new() -> Self {
        Self {
            b: true,
            c: "op".to_string()
        }
    }

    fn getstring(&mut self) -> String {
        std::mem::replace(&mut self.c, "".to_string())
    }
}

fn swapstr(mut e1: X, mut e2: Y) -> (X, Y) { //WTF
    let s1 = e1.getstring();
    let s2 = e2.getstring();
    e1.s = s2;
    e2.c = s1;
    (e1, e2)
}

#[derive(Debug)]
enum C {
    C1(i32, i32),
    C2(bool, String)
}

#[derive(Debug)]
struct D {
    c: C,
    s: String
}

impl D {
    fn new() -> Self {
        Self {
            c: C::C1(0,0),
            s: "".to_string()
        }
    }
    fn new_with_C(c: C) -> Self {
        let s = match &c {
            C::C1(_, _) => {"not there".to_string()},
            C::C2(_, s) => {s.clone()}
        };
        Self {
            c,
            s
        }
    }

    fn larger(&self) -> usize {
        self.s.len()
    }
}

impl fmt::Display for D {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "D {} with {:?}", self.s, self.c)
    }
}

fn swapelconcat(v: &mut Vec<String>, i: usize, j: usize) -> Option<&Vec<String>> {
    v[i] = v[i].clone()+&v[j];
    v[j] = v[j].clone()+&v[i];
    Some(v)
}

fn veclengths(v: &Vec<String>) -> Vec<i32> {
    v.iter().map(|el| {
        el.len() as i32
    }).collect()
}

fn removeelfromvector(v: &mut Vec<String>, n: usize) {
    let pos = v.iter().position(|el| {
        el.len() == n
    });
    v.remove(pos.unwrap());
}

pub fn main(){
    // let xx1 = XX::Y1(2,3);
    // println!("data {}",data(&xx1));
    // let xx1 = XX::Y2(String::from("test"));
    // println!("data {}",data(&xx1));

    // let z1 = Z::Y1(1,2);
    // let z2 = Z::Y2(true,String::from("new"));
    // println!("len {:?}", maybelength(&z1));
    // println!("len {:?}", maybelength(&z2));

    // let ex = enumx::X::Y(String::from("test"));
    // let sx = structx::X{i:String::from("asd")};
    // println!("Longer {}", modfun::longer(&ex,&sx));
    // let ex = enumx::X::Y(String::from("asdasd"));
    // println!("Longer {}", modfun::longer(&ex,&sx));

    // println!("Maybesqrt 4 {:?} ", maybesqrt(4));
    // println!("Maybesqrt -4 {:?} ", maybesqrt(-4));

    // let b = Balance{amt:100};
    // println!("maybewith {:?}", b.maybewithdraw(10));
    // println!("maybewith {:?}", b.maybewithdraw(200));

    // println!("char {}, prev {}", 'c', prevchar('c'));
    // println!("char {}, prev {}", 'a', prevchar('a'));
    // println!("char {}, prev {}", 'z', prevchar('z'));
    // let mut s = String::from("Pign");
    // println!("S {}",s);
    // let ret = replwithprev(&mut s);
    // println!("Returned: {:?}",ret);
    // let mut s = String::from("pigna");
    // println!("S {}",s);
    // let ret = replwithprev(&mut s);
    // println!("Returned: {:?}",ret);

    // let x = X::new();
    // let y = Y::new();
    // println!("X {:?} - Y {:?}", x, y);
    // let (x,y) = swapstr(x,y);
    // println!("X {} - Y {}", x, y);

    // let c1 = C::C1(0,1);
    // let c2 = C::C2(true, String::from("no way jose"));
    // println!("gotten {:?}",D::new());
    // let d1 = D::new_with_C(c1);
    // println!("dbg {:?}",d1);
    // println!("fmt {}",d1);
    // let d2 = D::new_with_C(c2);
    // println!("dbg {:?}",d2);
    // println!("fmt {}",d2);
    // println!("larger {}",d1.larger());
    // println!("larger {}",d2.larger());

    // let mut v1 = vec![String::from("some");12];
    // println!("Lengths {:?}", veclengths(&v1));

    // let mut v: Vec<String> = vec![String::from("what");4];
    // v.push(String::from("now"));    v.push(String::from("what"));
    // println!("{:?}",v);
    // removeelfromvector(&mut v, 3);
    // println!("{:?}",v);
}