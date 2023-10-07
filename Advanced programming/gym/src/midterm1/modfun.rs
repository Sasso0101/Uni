use super::{enumx, structx};


pub fn longer(s1: &enumx::X, s2: &structx::X) -> usize {
    match s1 {
        enumx::X::Y(s1) => {
            if (*s1).len() > (*s2).i.len() {
                (*s1).len()
            } else {
                (*s2).i.len()
            }
        }
    }
}