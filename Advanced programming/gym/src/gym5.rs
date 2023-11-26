use std::{fmt::{Debug, Display}, ops::{Add, Sub, Mul}};

use rand::{distributions::{Alphanumeric, DistString}, random, Rng};

trait Printable {
    fn print(&self);
}

impl Printable for i32 {
    fn print(&self) {
        println!("{self}")
    }
}
impl Printable for String {
    fn print(&self) {
        println!("{self}")
    }
}
impl<T: Printable> Printable for Vec<T> {
    fn print(&self) {
        for el in self {
            el.print()
        }
    }
}

// Monomorphic version
fn print_static(el: impl Printable) {
    el.print();
}

fn print_dynamic(el: &dyn Printable) {
    el.print();
}

#[derive(Debug, Default)]
enum Category {
    #[default]
    CAT1,
    CAT2,
    CAT3,
}
#[derive(Debug)]
struct Book {
    title: String,
    cat: Category,
}

impl Book {
    fn default_with_cat(cat: Category) -> Self {
        Self {
            cat,
            ..Self::default()
        }
    }
}

impl Default for Book {
    fn default() -> Self {
        let string = Alphanumeric.sample_string(&mut rand::thread_rng(), 16);
        Self {
            title: string,
            cat: Category::default(),
        }
    }
}

#[derive(Debug, Default)]
struct Library {
    bookcases: [Vec<Book>; 10],
}

trait Populatable {
    fn populate(&mut self);
}

impl Populatable for Library {
    fn populate(&mut self) {
        for bookcase in self.bookcases.iter_mut() {
            for _ in 0..3 {
                bookcase.push(Book::default())
            }
        }
    }
}

fn restricted<T: Debug + PartialOrd, U: Display>(t1: T, t2: T, u: U) {
    if t1 < t2 {
        println!("minor: {:?}", t1)
    } else {
        println!("minor {:?}", t2)
    }
    println!("u {}", u)
}

struct Tasks {
    tasks: Vec<Task>,
}

impl Tasks {
    pub fn new() -> Self {
        Self {
            tasks: vec![]
        }
    }
}

#[derive(Debug)]
struct Task {
    name: String,
    priority: i32,
    done: bool,
}

impl Task {
    pub fn new(name: &str, priority: i32, done: bool) -> Self {
        Self {
            name: name.to_string(),
            priority,
            done,
        }
    }
}

impl Iterator for Tasks {
    type Item = Task;
    fn next(&mut self) -> Option<Self::Item> {
        self.tasks
            .iter()
            .position(|t| !t.done)
            .map(|i| self.tasks.remove(i))
    }
}

struct Pair(i32, String);

impl Add<i32> for Pair {
    type Output = Pair;
    fn add(mut self, rhs: i32) -> Self::Output {
        self.0 += rhs;
        self
    }
}

impl Sub<i32> for Pair {
    type Output = Pair;
    fn sub(mut self, rhs: i32) -> Self::Output {
        self.0 -= rhs;
        self
    }
}

impl Add<&str> for Pair {
    type Output = Pair;
    fn add(mut self, rhs: &str) -> Self::Output {
        self.1.push_str(rhs);
        self
    }
}

impl Sub<&str> for Pair {
    type Output = Pair;
    fn sub(mut self, rhs: &str) -> Self::Output {
        self.1 = self.1.replace(rhs, "");
        self
    }
}

impl Add for Pair {
    type Output = Pair;
    fn add(self, rhs: Self) -> Self::Output {
        self + rhs.0 + rhs.1.as_str()
    }
}
impl Sub for Pair {
    type Output = Pair;
    fn sub(self, rhs: Self) -> Self::Output {
        self - rhs.0 - rhs.1.as_str()
    }
}

impl Mul<i32> for Pair {
    type Output = Self;
    fn mul(self, rhs: i32) -> Self::Output {
        Pair(self.0.pow(rhs as u32), self.1.repeat(rhs as usize))
    }
}

struct Open;
impl Gate<Open> {
    fn close(self) -> Result<Gate<Closed>, Gate<Stopped>> {
        let rand = rand::thread_rng().gen_bool(0.7);
        match rand {
            true => Ok(Gate {state: Closed}),
            false => Err(Gate {state: Stopped { reason: "idk".to_string() }})
        }
    }
}
struct Closed;
impl Gate<Open> {
    fn open(self) -> Result<Gate<Open>, Gate<Stopped>> {
        let rand = rand::thread_rng().gen_bool(0.7);
        match rand {
            true => Ok(Gate {state: Open}),
            false => Err(Gate {state: Stopped { reason: "idk".to_string() }})
        }
    }
}
struct Stopped {
    reason: String
}
impl Gate<Stopped> {
    fn open(self) -> Gate<Open> {
        Gate {state: Open}
    }
    fn close(self) -> Gate<Closed> {
        Gate {state: Closed}
    }
}
struct Gate<S> {
    state: S
}

trait Heatable {
    fn cook(self);
}
trait Friable {
    fn cook(self);
}
trait Heater<T: Heatable> {
    fn heat(&self, heatable: T);
}
trait Fryer<T: Friable> {
    fn heat(&self, friable: T);
}
struct Oven;
struct Pan;
impl<T: Heatable> Heater<T> for Oven {
    fn heat(&self, heatable: T) {
        heatable.cook();
    }
}
impl<T: Friable> Fryer<T> for Pan {
    fn heat(&self, friable: T) {
        friable.cook();
    }
}
struct Pie {
    ready: bool
}
struct Carrot {
    state: CarrotState
}
enum CarrotState {Raw, Cooked, Fried, Burnt}
trait Edible {
    fn eat(self);
}
impl Heatable for Pie {
    fn cook(mut self) {
        match self.ready {
            true => println!("You burnt the pie!"),
            false => self.ready = true
        }
    }
}
impl Heatable for Carrot {
    fn cook(mut self) {
        match self.state {
            CarrotState::Raw => self.state = CarrotState::Cooked,
            _ => self.state = CarrotState::Burnt
        }
    }
}
impl Friable for Carrot {
    fn cook(mut self) {
        match self.state {
            CarrotState::Fried => self.state = CarrotState::Burnt,
            _ => self.state = CarrotState::Fried
        }
    }
}
impl Edible for Pie {
    fn eat(self) {
        match self.ready {
            false => println!("You've got stomach ache!"),
            true => println!("Yummy!")
        }
    }
}
impl Edible for Carrot {
    fn eat(self) {
        match self.state {
            CarrotState::Raw => println!("mmh, crunchy"),
            CarrotState::Cooked => println!("mmh, yummy"),
            CarrotState::Fried => println!("mmh, crispy"),
            CarrotState::Burnt => println!("mmh, burnt"),
        }
    }
}
#[cfg(test)]
mod tests {
    use super::{Tasks, Task};

    #[test]
    fn exercise4() {
        let mut tasks = Tasks::new();
        tasks.tasks.push(Task::new("test", 1, false));
        tasks.tasks.push(Task::new("tes1", 1, false));
        tasks.tasks.push(Task::new("tes2", 1, false));
        tasks.tasks.push(Task::new("tes3", 1, false));
        println!("{:?}", tasks.next());
        println!("{:?}", tasks.next());
        println!("{:?}", tasks.next());
        println!("{:?}", tasks.next());
    }
}
