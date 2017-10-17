#!/bin/bash

#REALKEY=8a223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa
HEX="a b c d e f 0 1 2 3 4 5 6 7 8 9"

KEYSUFFIX=0b76bf1952cf38b9f6f4e5da73b7ed6d9b3dfb18f5a1a0811713b747
IV=7894e53d44a5e9ecd741f62c93511e2f
CIPHER=fnV6S8Hz0BpBkiB76OEDbMyEDN5nkxTeEGXj5Ij3zKTiZwGMNX+VTOm7BCttEVw5GVPVEvBmKIGC7ZohugUhvwSDpMO4yEVb1txvPJGT0qJP0IXspY3sGkF1w8hR+kWMRWeb1iPunXNrTljEZ/T/DpO8ZupARBKLUF2zyS0jFWC6MFQ+Hg1zRZdGlfP6Jx4qSmH2+eA6g0/OV0wjAlZjjt+74gl1DSydtJzQfpH+/ha3abAPRrriIyidYxWesGpe


function check()
{
    openssl des -d -aes-256-cbc -base64 -K $1 -iv $IV <<< $CIPHER 2>/dev/null | grep -a -P '\X{20}'
}

function findText()
{
    for b in $HEX
    do
		for c in $HEX 
		do
			for d in $HEX
			do
				for e in $HEX 
				do
					for f in $HEX
					do
						for g in $HEX 
						do
							for h in $HEX
							do
       							check $1$b$c$d$e$f$g$h$KEYSUFFIX
							done
						done
					done
				done
			done
		echo -ne "\r$1$b$c"
		done
    done
}

for a in $HEX
do
	findText $a &
done

wait
