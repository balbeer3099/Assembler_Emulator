; let's check comment works or not
start:
ldc 0
adc 2
adc 2
adc -1
adc 2
var1: SET 3 ; set value of var1 as 3
adc var1 ; use var1 to add 3
end:
HALT