#!/bin/bash
./demo -h
./demo adduser 001 001
./demo adduser 002 002
./demo showuser
./demo deluder 1
./demo showuser
./demo 001 001 addtask 999 2023:18:12:23:67:12
./demo 001 001 addtask 999 2022:11:12:45:12:12
./demo 001 001 addtask 111 2023:07:10:20:12:23 -c study
./demo 001 001 addtask 222 2023:07:10:22:10:23 -c home
./demo 001 001 addtask 333 2023:09:10:11:23:14 -c study
./demo 001 001 showtask
./demo 001 001 showtask -date 2023:07:10
./demo 001 001 showtask -month 2023:07
./demo 001 001 showtask -category study
./demo 001 001 deltask 2
./demo 001 001 showtask 

