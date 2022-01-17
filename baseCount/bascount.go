package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

type datf struct {
	a      uint64
	t      uint64
	g      uint64
	c      uint64
	n      uint64
	gc     float64
	seqlen uint64
}

func main() {
	fp := os.Args[1]
	content, contentEr := ioutil.ReadFile(fp)
	if contentEr != nil {
		panic(contentEr.Error())
	}

	chrs := make(map[string]datf, 100)
	var chrOrder []string
	buf := strings.Split(string(content), ">")

	for _, each := range buf {
		lineUp := strings.ToUpper(strings.TrimSpace(each))
		if len(lineUp) == 0 {
			continue // skip first header split
		}

		var seqThis datf = datf{0, 0, 0, 0, 0, 0, 0}
		tmp := strings.SplitN(each, "\n", 2)
		head := strings.Split(tmp[0], " ")[0]
		seq := tmp[1]

		seqThis.a = uint64(strings.Count(seq, "A"))
		seqThis.t = uint64(strings.Count(seq, "T"))
		seqThis.g = uint64(strings.Count(seq, "G"))
		seqThis.c = uint64(strings.Count(seq, "C"))
		seqThis.n = uint64(strings.Count(seq, "N"))
		seqThis.seqlen = seqThis.g + seqThis.c + seqThis.a + seqThis.t + seqThis.n
		seqThis.gc = float64(seqThis.g+seqThis.c) / float64(seqThis.seqlen)
		chrs[head] = seqThis
		chrOrder = append(chrOrder, head)
		fmt.Println(seqThis)
	}
	var seqAll datf = datf{0, 0, 0, 0, 0, 0, 0}
	for _, v := range chrOrder {
		seqAll.a += chrs[v].a
		seqAll.t += chrs[v].t
		seqAll.g += chrs[v].g
		seqAll.c += chrs[v].c
		seqAll.n += chrs[v].n
		seqAll.seqlen += chrs[v].seqlen
		seqAll.gc = float64(seqAll.g+seqAll.c) / float64(seqAll.seqlen)
	}
	chrs["ALL"] = seqAll
	chrOrder = append(chrOrder, "ALL")
	fmt.Printf("chr_Name\tbase_A\tbase_T\tbase_G\tbase_C\tbase_N\tGC_Rate\tseq_Len\n")
	for _, v := range chrOrder {
		datfShow(v, chrs[v])
	}
}

func datfShow(chrname string, x datf) {
	gc := fmt.Sprintf("%.2f", x.gc*100)
	fmt.Printf("%s\t%d\t%d\t%d\t%d\t%d\t%s%%\t%d\n", chrname, x.a, x.t, x.g, x.c, x.n, gc, x.seqlen)
}
