// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string)

	Counter          = 0
	userslist        = make(map[string]string)
	administrator    = ""
	nametag          = make(map[string]net.Conn)
	logOfConnections = make(map[string]string)
)

//functions
func displayInfo(to, usr string) {
	privMessageSender("irc-server", to, "username: "+usr+" IP: "+strings.Split(nametag[usr].LocalAddr().String(), ":")[0]+" Connected at: "+logOfConnections[usr])

}

func displayTime(to string) {
	location, err := time.LoadLocation("America/Mexico_City ")
	if err != nil {
		panic(err)
	}
	time := time.Now().In(location)
	privMessageSender("IRc-SERVER", to, "local-time: "+time.Location().String()+time.Format("15:04"))

}

func displayUserList(to string) {
	for usr := range nametag {
		privMessageSender("irc-server", to, usr+" - connected since "+logOfConnections[usr]+"\n")
	}

}

func kick(usr string) {
	a := nametag[usr]
	if a != nil {
		nametag[usr].Close()
		fmt.Printf("[%s] was kicked\n", usr)
		delete(userslist, nametag[usr].RemoteAddr().String())
		delete(nametag, usr)

	}
}
func privMessageSender(from, to, message string) {
	ch := make(chan string)
	go clientWriter(nametag[to], ch)
	ch <- from + " > " + message
	close(ch)
	return
}

// skeleton of chat
func broadcaster() {
	sender := ""
	message := ""
	clients := make(map[client]bool)
	standard := false

	for {
		standard = false
		select {
		case msg := <-messages:
			values := []rune(msg)
			a := strings.Index(msg, ":")
			if a >= 0 {
				message = string(values[a+2:])
				sender = string(values[:a])
				standard = true
			}

			messageInfo := strings.SplitN(message, " ", 2)
			if len(messageInfo[0]) > 0 {
				if messageInfo[0][0] == '/' {
					switch messageInfo[0] {
					case "/users":
						displayUserList(sender)
					case "/msg":
						if len(messageInfo) < 2 {
							break
						}
						messageData := strings.SplitN(messageInfo[1], " ", 2)
						if len(messageInfo) < 2 {
							break
						}
						privMessageSender(sender, messageData[0], messageData[1])
					case "/time":

						displayTime(sender)
					case "/user":
						if len(messageInfo) < 2 {
							break
						}
						displayInfo(sender, messageInfo[1])
					case "/kick":
						if len(messageInfo) < 2 {
							break
						}
						if nametag[sender].RemoteAddr().String() == administrator {
							kick(messageInfo[1])

							for cli := range clients {
								cli <- "[" + messageInfo[1] + "] was kicked "
							}
						}
					}

				} else {
					for cli := range clients {

						if standard {
							cli <- sender + " > " + message
						} else {
							cli <- msg
						}
					}

				}

			} else {
				// Broadcast incoming message to all
				// clients' outgoing message channels.
				for cli := range clients {

					if standard {
						cli <- sender + " > " + message
					} else {
						cli <- msg
					}
				}

			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	time := time.Now()
	actual := conn.RemoteAddr().String()
	input := bufio.NewScanner(conn)
	input.Scan()
	userName := input.Text()
	Counter++
	userslist[actual] = userName
	nametag[userName] = conn
	logOfConnections[userName] = time.Format("2007-03-02 12:01:07")
	fmt.Printf("irc-server > New connected user [%s]\n", userslist[actual])
	ch <- "irc-server > Welcome to the Simple IRC Server"
	ch <- "irc-server > Your user [" + userslist[actual] + "] is successfully logged"
	if administrator == "" {
		administrator = actual
		ch <- "irc-server > Congrats, you were the first user"
		ch <- "irc-server > You're the new IRC Server administrator"
		fmt.Printf("irc-server > [%s] was promoted as the channel administrator\n", userslist[actual])
	}

	messages <- userslist[actual] + " has arrived"
	entering <- ch

	//input := bufio.NewScanner(conn)
	for input.Scan() {
		messages <- userslist[actual] + ": " + input.Text()

	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	//messages <- actual + " has left"

	messages <- "irc-server > " + userslist[actual] + " left the channel"
	fmt.Printf("irc-server > [%s] left\n", userslist[actual])

	a := nametag[userslist[actual]]
	if a != nil {
		delete(nametag, userslist[actual])
		delete(userslist, actual)

	}

	if actual == administrator {
		for nam := range nametag {
			privMessageSender("irc-server", nam, "You're the new IRC Server administrator")
			fmt.Printf("irc-server > [%s] was promoted as the channel administrator\n", userslist[nametag[nam].RemoteAddr().String()])
			administrator = nametag[nam].RemoteAddr().String()

		}
	}

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	listener, err := net.Listen("tcp", os.Args[2]+":"+os.Args[4])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("irc-server > Simple IRC Server started at %s\n", listener.Addr())

	go broadcaster()
	fmt.Print("irc-server > Ready for receiving new clients\n")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}

}
