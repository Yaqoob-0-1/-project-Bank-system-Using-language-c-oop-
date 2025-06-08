#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "../Librarys/stringo.h"

// global variable
const string filePath = "../data/Currencies.txt"; // currency data file path 
const string sep = "#//#"; // seperator

class cCurrency
{
private:

	// emun mode
	enum Mode
	{
		eEmptyObj = 0,
		eUpdateObj = 1,
	};

	Mode _mode;
	string _Country;
	string _CurrencyCode;
	string _CurrencyName;
	float _Rate;

	//=====================================================================//
	//							Private Methods

	// convert dataLine to currencyObj
	static cCurrency _convertLinetoCurrencyObj(string line)
	{
		vector<string> dataLine = stringo::split(line, sep);
		return cCurrency(Mode::eUpdateObj, dataLine[0],dataLine[1],dataLine[2],stof(dataLine[3]));
	}
	// convert currencyObj to dataLine
	string _convertCurrencyObjtoLine(cCurrency & currencyObj)
	{
		string line;
		line += currencyObj.country + sep;
		line += currencyObj.currencyCode + sep;
		line += currencyObj.currencyName + sep;
		line += to_string(currencyObj.rate);

		return line;
	}

	// get data from file
	static vector<cCurrency> _LoadCurrencyDataFromFile()
	{
		vector<cCurrency> _currencyData;

		fstream file(filePath, ios::in);
		if (file.is_open())
		{
			string line;
			while (getline(file,line))
			{
				cCurrency currency = _convertLinetoCurrencyObj(line);
				_currencyData.push_back(currency);
			}
			file.close();
		}
		return _currencyData;
	}
	// save currencyObjs to file
	void _saveCurrencyDatatoFile(vector<cCurrency> & currencyData)
	{
		fstream file(filePath, ios::out);
		if (file.is_open())
		{
			for(cCurrency & currency : currencyData)
			{
				file << _convertCurrencyObjtoLine(currency) << endl;
			}
		}
		file.close();
	}

	// updata data in file
	void _update()
	{
		// load data from file
		vector<cCurrency> _CurrencyData = _LoadCurrencyDataFromFile();

		// updata data in vector
		for (cCurrency& currency : _CurrencyData)
		{
			if (currency.country == _Country)
			{
				currency = *this;
				break;
			}
		}
		// save data to file
		_saveCurrencyDatatoFile(_CurrencyData);
	}
	// get EmptyObject
	static cCurrency _getEmptyObj()
	{
		return cCurrency(Mode::eEmptyObj, "", "", "", 0);
	}


public:

	// Parameterized Constructor
	cCurrency(Mode mode, string country, string currencyCode, string currencyName, float rate)
	{
		_mode = mode;
		_Country = country;
		_CurrencyCode = currencyCode;
		_CurrencyName = currencyName;
		_Rate = rate;
	}

	// properties getters
	string Country()
	{
		return _Country;
	}
	string CurrencyCode()
	{
		return _CurrencyCode;
	}
	string CurrencyName()
	{
		return _CurrencyName;
	}
	float Rate()
	{
		return _Rate;
	}

	_declspec(property(get = Country)) string country;
	_declspec(property(get = CurrencyCode)) string currencyCode;
	_declspec(property(get = CurrencyName)) string currencyName;
	_declspec(property(get = Rate)) float rate;

	//=========================================================================//
	//								Methods

	bool isEmpty()
	{
		return (_mode == Mode::eEmptyObj);
	}
	void updateRate(float newRate)
	{
		_Rate = newRate;
		_update(); // update file
	}

	//=========================================================================//
	//							Static Methods

	// return currency obj if found or emptyObj if not
	static cCurrency findByCode(string currencyCode)
	{
		vector<cCurrency> currencyData = _LoadCurrencyDataFromFile();
		currencyCode = stringo::upper(currencyCode);
		for (cCurrency &currency : currencyData)
		{
			if (stringo::upper(currency.currencyCode) == currencyCode)
			{
				return currency; // if found currency
			}
		}
		// if not found currency
		return _getEmptyObj();
	}
	// return currency obj if found or emptyObj if not
	static cCurrency findByCountry(string country)
	{
		vector<cCurrency> currencyData = _LoadCurrencyDataFromFile();
		country = stringo::upper(country);
		for (cCurrency &currency : currencyData)
		{
			if (stringo::upper(currency.country) == country)
			{
				return currency; // if found currency
			}
		}
		// if not found currency
		return _getEmptyObj();
	}
	// return true if currnecy is exist or false if not found
	static bool isExist(string country)
	{
		return !(findByCountry(country).isEmpty());
	}
	// return true if currnecy is exist or false if not found
	static bool isExistCode(string currnceyCode)
	{
		return !(findByCode(currnceyCode).isEmpty());
	}
	// return all currrencies
	static vector<cCurrency> getAllCurrencies()
	{
		return _LoadCurrencyDataFromFile();
	}

};


#pragma once  // تضمين الملف مرة واحدة فقط لمنع التكرار

#include <iostream>   // مكتبة الإدخال والإخراج الأساسية
#include <fstream>    // مكتبة التعامل مع الملفات
#include <vector>     // مكتبة المصفوفات الديناميكية
#include "../Librarys/stringo.h"  // مكتبتنا المساعدة لمعالجة النصوص

// مسار ملف بيانات العملات
const string filePath = "../data/Currencies.txt";  
// الفاصل المستخدم في ملف البيانات
const string sep = "#//#";  

class cCurrency {
private:
    // أنواع الكائنات المتاحة
    enum Mode {
        eEmptyObj = 0,  // كائن فارغ
        eUpdateObj = 1, // كائن يحتوي على بيانات
    };

    Mode _mode;          // حالة الكائن الحالي
    string _Country;     // اسم الدولة
    string _CurrencyCode; // رمز العملة (مثل USD)
    string _CurrencyName; // اسم العملة الكامل
    float _Rate;         // سعر الصرف

    // تحويل سطر من الملف إلى كائن عملة
    static cCurrency _convertLinetoCurrencyObj(string line) {
        vector<string> dataLine = stringo::split(line, sep);  // تقسيم السطر باستخدام الفاصل
        return cCurrency(Mode::eUpdateObj, dataLine[0], dataLine[1], dataLine[2], stof(dataLine[3]));
    }

    // تحويل كائن عملة إلى سطر نصي للحفظ في الملف
    string _convertCurrencyObjtoLine(cCurrency &currencyObj) {
        string line;
        line += currencyObj.country + sep;       // إضافة الدولة
        line += currencyObj.currencyCode + sep;  // إضافة رمز العملة
        line += currencyObj.currencyName + sep;  // إضافة اسم العملة
        line += to_string(currencyObj.rate);     // إضافة سعر الصرف
        return line;
    }

    // تحميل بيانات العملات من الملف
    static vector<cCurrency> _LoadCurrencyDataFromFile() {
        vector<cCurrency> _currencyData;  // مصفوفة لحفظ البيانات
        
        fstream file(filePath, ios::in);  // فتح الملف للقراءة
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {  // قراءة الملف سطراً سطراً
                cCurrency currency = _convertLinetoCurrencyObj(line);  // تحويل السطر لكائن
                _currencyData.push_back(currency);  // إضافة الكائن للمصفوفة
            }
            file.close();  // إغلاق الملف
        }
        return _currencyData;
    }

    // حفظ بيانات العملات إلى الملف
    void _saveCurrencyDatatoFile(vector<cCurrency> &currencyData) {
        fstream file(filePath, ios::out);  // فتح الملف للكتابة
        if (file.is_open()) {
            for(cCurrency &currency : currencyData) {
                file << _convertCurrencyObjtoLine(currency) << endl;  // كتابة كل كائن كسطر
            }
        }
        file.close();  // إغلاق الملف
    }

    // تحديث بيانات العملة في الملف
    void _update() {
        vector<cCurrency> _CurrencyData = _LoadCurrencyDataFromFile();  // تحميل البيانات
        
        // البحث عن العملة وتحديثها
        for (cCurrency& currency : _CurrencyData) {
            if (currency.country == _Country) {
                currency = *this;  // استبدال البيانات القديمة بالجديدة
                break;
            }
        }
        
        _saveCurrencyDatatoFile(_CurrencyData);  // حفظ التغييرات
    }

    // إنشاء كائن عملة فارغ
    static cCurrency _getEmptyObj() {
        return cCurrency(Mode::eEmptyObj, "", "", "", 0);
    }

public:
    // البناء (يأخذ جميع خصائص العملة)
    cCurrency(Mode mode, string country, string currencyCode, string currencyName, float rate) {
        _mode = mode;
        _Country = country;
        _CurrencyCode = currencyCode;
        _CurrencyName = currencyName;
        _Rate = rate;
    }

    // واجهة للوصول إلى الخصائص (للقراءة فقط)
    string Country() { return _Country; }
    string CurrencyCode() { return _CurrencyCode; }
    string CurrencyName() { return _CurrencyName; }
    float Rate() { return _Rate; }

    // تعريف خاصيات للوصول إلى الدوال السابقة بأسماء مختصرة
    _declspec(property(get = Country)) string country;
    _declspec(property(get = CurrencyCode)) string currencyCode;
    _declspec(property(get = CurrencyName)) string currencyName;
    _declspec(property(get = Rate)) float rate;

    // ========== الدوال العامة ========== //

    // التحقق إذا كان الكائن فارغاً
    bool isEmpty() {
        return (_mode == Mode::eEmptyObj);
    }


    // ========== الدوال الثابتة ========== //


};

