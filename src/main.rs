
use std::fs::File;
use std::io::prelude::*;
use std::process;

fn main() {
    //read key from file

    let temp = File::open("SlackToken");
    let mut file;
    if temp.is_ok() {
        file = temp.ok().unwrap();
    } else {
        println!("Cant open token file, {}", temp.err().unwrap());
        process::exit(1);
    }
    let mut token = String::new();
    let _ = file.read_to_string(&mut token);
    println!("your Slack token is \"{}\"", token);

    //rtm.connect

    let url = format!("https://slack.com/api/rtm.connect?token={}", token);
    println!("rtm connect URL is: \"{}\"", url);

    

    //read res from ^
    //open webhook url from res
    //print messages as the come
}