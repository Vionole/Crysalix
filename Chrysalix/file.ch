
var: $1, ARR;
var: $2, ARR;
var: $3, ARR;

pushb: $1, 'string';
pushb: $1, 3.14;
pushb: $1, -9;

pushb: $2, true;
pushb: $2, false;
pushb: $2, nil;


pushb: $2, $1;

pushb: $3, $1;
pushb: $3, $2;
pushb: $3, $1;

print: $3, '\n';

var: $value, 666;
size: $value, $2;
setval: $value, $5, 1, 3, 1;

print: $value, '\n';
##
	#Циклический расчет факториала:
	var: $n, ntg16; 	#Факториал чего считаем
	var: $i, ntg 1; 	#С какого числа ведем итерацию
	var: $result, ntg1; 	#Переменная результата
	var: $j, ntg1; 		#Итератор главного цикла
	var: $max, ntg100; 	#Максимальне количество итераций главного цикла.
	newtemp: ntg 1;
label: &main_loop;
	print: 'Введите число, по которому будет расчет факториала: ';
	input:$n;
	to: 'untg', $n; 

	newtemp: ntg 1;
	label: &cycle_start;
		calc: '*', $result, $i;
		calc: 'inc', $i;
		comp: '<=', $1, $i, $n; 
	jif: $1, &cycle_start;
	forget: ntg 1;	

	print: 'Факториал ', $n, ' равен ', $result, '\n';
	
	#Сбрасывем значения переменных перед следующей итерации
	change: $i, ntg 1;
	change: $result, ntg1;

	calc: 'inc', $j;
	comp: '<', $0, $j, $max; 
jif: $0, &main_loop;	
	
		#Освобождаем память, конец программы
	forget:;
	free: $n, $i, $result, $j, $max;
pause:;
##
end: ntg 0;