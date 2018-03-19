extern crate futures;
extern crate hyper;
extern crate tokio_core;

use std::fs::File;
use std::io::prelude::*;
use std::process;
use std::io::{self, Write};
use futures::{Future, Stream};
use hyper::Client;
use tokio_core::reactor::Core;


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
    //build url
    let url = format!("http://slack.com/api/rtm.connect?token={}", token);
    println!("rtm connect URL is: \"{}\"", url);


    let temp = Core::new();
    let mut core;
    if temp.is_ok(){
        core = temp.ok().unwrap();
    } else {
        println!("error creating core, {}", temp.err().unwrap());
        process::exit(1);
    }
    let client = Client::new(&core.handle());
    let temp = url.parse();
    let uri: hyper::Uri;
    if temp.is_ok() {
        uri = temp.ok().unwrap();
    } else {
        println!("error parsing URL, {}", temp.err().unwrap());
        process::exit(1);
    }

    let work = client.get(uri).map(|res| {
        println!("Response: {}", res.status());

        res.body().for_each(|chunk| {
        io::stdout()
            .write_all(&chunk)
            .map_err(From::from)
        })
    });

    let temp = core.run(work);
    if temp.is_ok(){
        println!("Run Ok");
    } else {
        println!("Error in run, {}", temp.err().unwrap());
        process::exit(1);
    }

    //read res from ^
    //open webhook url from res
    //print messages as the come
}