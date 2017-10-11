#!/bin/bash

REALKEY=8a223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa
KEYSUFFIX=223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa
HEX="0 1 2 3 4 5 6 7 8 9 a b c d e f"

function check()
{
    
    openssl des -d -aes-256-cbc -base64 -K $1 -iv df3b9714250aac834c2862a001d79464 <<< og6h+ctLc+VFEifyV1n2Mi6lqyWJIra44FlsgHa9+RNn5yqsAAoie3ypAaw7kYtlHeRfSjREMPQY4YnRouy3Z8t/VEioxdYm8s7Y+BDJwV4SDvWNRq/o4+5p+gQosJAja2jcoYk2x8HMszR/+haX4Q== 2>/dev/null | grep -a -P '\X{12}'
}

for a in $HEX 
do
    for b in $HEX
    do
        check $a$b$KEYSUFFIX &
    done
done

wait
