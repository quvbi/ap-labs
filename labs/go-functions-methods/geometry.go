// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

func randFloat(min, max float64, n int) float64 {
	// res := float64
	return min + rand.Float64()*(max-min)

}

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

func (point Point) Y() float64 {
	return point.y
}

func (point Point) X() float64 {
	return point.x
}

func orientacion(a, b, c Point) int {
	valor := (b.y-a.y)*(c.x-b.x) - (b.x-a.x)*(c.y-b.y)

	if valor == 0 {

		return 0

	} else if valor > 0 {
		return 1

	} else {

		return 2
	}
}

func enSegmento(a Point, b Point, c Point) bool {

	if b.x <= math.Max(a.x, c.x) && b.x >= math.Min(a.x, c.x) &&
		b.y <= math.Max(a.y, c.y) && b.y >= math.Min(a.y, c.y) {

		return true

	}

	return false
}

func interseccion(a, b, a1, b1 Point) bool {

	ore := orientacion(a, a1, b)

	ore1 := orientacion(a, a1, b1)

	ore2 := orientacion(b, b1, a)

	ore3 := orientacion(b, b1, a1)

	if (ore != ore1) && (ore2 != ore3) {
		return true
	}

	if (ore == 0) && enSegmento(a, b, a1) {
		return true
	}
	if (ore1 == 0) && enSegmento(a, b1, a1) {
		return true
	}
	if (ore2 == 0) && enSegmento(b, a, b1) {
		return true
	}
	if (ore3 == 0) && enSegmento(b, a1, b1) {
		return true
	}

	return false

}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func main() {

	lados,
		err := strconv.Atoi(os.Args[1])

	if lados <= 2 {
		fmt.Println("Una figura tiene mas de 2 lados")
		return
	} else if err != nil {
		fmt.Println(err)
		return
	}

	noInt := false

	puntos := make(Path, lados)

	fmt.Printf("- Generating a [%d] sides figure\n", lados)

	for {

		rand.Seed(time.Now().UnixNano())

		for i := 0; i < lados; i++ {
			puntos[i] = Point{
				randFloat(-100.0, 100.0, 1), randFloat(-100.0, 100.0, 1)}
		}

		for i := 0; i < lados; i++ {

			if interseccion(puntos[i%lados], puntos[(1+i)%lados], puntos[(2+i)%lados], puntos[(3+i)%lados]) {

				noInt = true

				break
			}
		}

		if noInt == false {

			fmt.Println("- Figure's vertices ")

			for i := range puntos {

				fmt.Printf("\t- ( %6.2f, %6.2f)\n", puntos[i].X(), puntos[i].Y())
			}

			fmt.Println("- Figure's perimeter: ")

			fmt.Printf("\t %0.2f\n", puntos.Distance())

			return

		} else {

			noInt = false

		}

	}
}

//!-path
