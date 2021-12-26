package main

import (
	"net"
	"fmt"
	"io"
	"os"
	"time"
)

func check_err(err error) {
	if err != nil {
		fmt.Println("err :",err)
		os.Exit(0)
	}
}

func copy(dest net.Conn, src net.Conn) {
	io.Copy(dest, src)
	fmt.Println("Copied !")
	return
}

func connServ() net.Conn {
	var (
		server_ip string = "127.0.0.1"
		server_port string = "8000"
	)
	target, err := net.Dial("tcp", server_ip+":"+server_port)
	check_err(err)
	return target
}

func main() {
	fmt.Println("connected to the server")
	// client handling loop
	var (
		client_ip string = "127.0.0.1"
		client_port string = "8080"
		target net.Conn
	)
	incoming, err := net.Listen("tcp", client_ip+":"+client_port)
	check_err(err)
	defer incoming.Close()
	for {
		target = connServ()
		client, err := incoming.Accept()
		check_err(err)
		fmt.Println("connected to a victim here")
		go copy(target, client)
		go copy(client, target)
		time.Sleep(5*time.Second)
		client.Close()
		target.Close()

	}
}
