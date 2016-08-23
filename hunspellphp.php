<?php
$time = microtime(true);
$a = new Hunspell("/usr/share/hunspell/pl_PL.aff", "/usr/share/hunspell/pl_PL.dic");
var_dump($a->encoding);
var_dump($a->spell("słońce"));
var_dump($a->stem("latamy"));
var_dump(array_map(function($n){
    return iconv('ISO-8859-2', 'UTF-8', $n);
}, $a->stem("latamy")));

var_dump($a->spell(iconv('UTF-8', 'ISO-8859-2', 'słońce')));
var_dump($a->suggest("test"));
var_dump($a->analyze("grali"));
var_dump($a->add("testyyyy"));
var_dump($a->spell("testyyyy"));
var_dump($a->remove("testyyyy"));
var_dump($a->spell("testyyyy"));

echo microtime(true) - $time;
