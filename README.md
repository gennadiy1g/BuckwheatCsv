# Buckwheat CSV

A viewer for CSV files.

## Why this program is useful

This program is useful if:

 * You want to preview a CSV file.
 * The file is too big for Microsoft Excel or LibreOffice Calc, or it takes too much time to open the file in one of these programs.

 Buckwheat CSV:
 
 * Will not load the whole file into memory. 
 * Does not have limit on the size of the file.
 * Has limit on the maximum number of rows or columns (see below).

## Limitations

* At this time, Buckwheat CSV is available only for 64-bit Windows.
* Maximum number of rows or columns is 2,147,483,647 (32-bit signed integer).
* CSV files must be [UTF-8](https://en.wikipedia.org/wiki/UTF-8) encoded.

## Getting started

* Download zip file of the latest release from [https://github.com/gennadiy1g/BuckwheatCsv/releases](https://github.com/gennadiy1g/BuckwheatCsv/releases).
* Unzip into a folder of your choice.
* Double click on BuckwheatCsv.exe.
* Click File->Open ..., or drag and drop a CSV file onto Buckwheat CSV's window.

## Credit and licenses for embedded code

This project includes code from outside sources:

* [Boost](https://www.boost.org/LICENSE_1_0.txt)
* [wxWidgets](https://www.wxwidgets.org/about/licence/)
