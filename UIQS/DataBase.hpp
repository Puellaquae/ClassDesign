﻿#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "LinkList.hpp"
#include "FileIO.hpp"

template<typename T>
class DataBase
{
	decltype(T::field)& field = T::field;
	const std::string file_path;

	LinkList<T> raw{};
	explicit DataBase(LinkList<T> list) :raw(std::move(list)) {}
	void load()
	{
		std::wifstream ifs = FileIO::file_auto_encoding_open(file_path);
		if (ifs.is_open() && !ifs.bad())
		{
			std::wstring line;
			while (std::getline(ifs, line))
			{
				raw.push_back(T(line));
			}
		}
		else
		{
			std::cerr << "Open File Fail";
		}
	}
public:
	void save()
	{
		if (file_path.empty())return;
		std::wofstream ofs(file_path, std::ios::out | std::ios::trunc);
		if (ofs.is_open())
		{
			for (const auto& it : raw)
			{
				ofs << static_cast<std::wstring>(it) << std::endl;
			}
		}
	}

	void save(const std::string& saveFilePath)
	{
		if (saveFilePath.empty())return;
		std::wofstream ofs(saveFilePath, std::ios::out | std::ios::trunc);
		if (ofs.is_open())
		{
			for (const auto& it : raw)
			{
				ofs << static_cast<std::wstring>(it) << std::endl;
			}
		}
	}

	~DataBase()
	{
		save();
	}
	void insert(const T& val)
	{
		raw.push_back(val);
	}
	int remove(std::function<bool(const T&)>where)
	{
		return raw.erase(where);
	}
	int update(std::wstring item, const std::wstring& newval, std::function<bool(const T&)>where)
	{
		if (std::find(field.begin(), field.end(), item) == field.end()) { return 0; }
		int res = 0;
		for (auto it = raw.begin(); it != raw.end(); ++it)
		{
			if (where(*it))
			{
				(*it)[item] = newval;
				res++;
			}
		}
		return res;
	}
	DataBase() = default;
	DataBase(const DataBase<T>& db) :DataBase(db.raw) {}
	DataBase(DataBase&& db) noexcept
	{
		raw = std::move(db.raw);
	}
	DataBase<T>& operator=(const DataBase<T>& db)
	{
		if (this != &db)
		{
			raw = db.raw;
		}
		return *this;
	}
	DataBase<T>& operator=(DataBase<T>&& db) noexcept
	{
		if (&db != this)
		{
			raw = std::move(db.raw);
		}
		return *this;
	}

	explicit  DataBase(std::string filepath) :file_path(std::move(filepath))
	{
		load();
	}
	DataBase<T> copy()
	{
		return DataBase<T>(raw);
	}
	DataBase<T> where(std::function<bool(const T& val)>pred)
	{
		DataBase<T> result;
		for (const auto& x : raw)
		{
			if (pred(x))
			{
				result.insert(x);
			}
		}
		return result;
	}
	template<typename Tresult>
	DataBase<Tresult> select(std::function<Tresult(const T&)>selector)
	{
		DataBase<Tresult> result;
		for (const auto& x : raw)
		{
			result.insert(selector(x));
		}
		return result;
	}
	DataBase<T> orderby(std::function<bool(const T&, const T&)> cmp)
	{
		DataBase<T> result(raw);
		result.raw.sort(cmp);
		return result;
	}
	DataBase<T> query(const std::wstring& cmd)
	{
		DataBase<T> res(raw);
		std::wstringstream cmdin(cmd);
		std::wstring cmdA;
		while (cmdin >> cmdA)
		{
			if (cmdA == L"选择")
			{
				std::wstring f, op, val;
				cmdin >> f >> op >> val;
				if (!cmdin.fail() && std::find(field.begin(), field.end(), f) != field.end())
				{
					if (op == L"等于")res = res.where([=](const T& u) {return u[f] == val; });
					else if (op == L"不等于")res = res.where([=](const T& u) {return u[f] != val; });
					else if (op == L"包含")res = res.where([=](const T& u) {return u[f].find(val) != u[f].npos; });
					else if (op == L"不包含")res = res.where([=](const T& u) {return u[f].find(val) == u[f].npos; });
					else if (op == L"大于")res = res.where([=](const T& u) {return u[f] > val; });
					else if (op == L"小于")res = res.where([=](const T& u) {return u[f] < val; });
					else if (op == L"不大于")res = res.where([=](const T& u) {return u[f] <= val; });
					else if (op == L"不小于")res = res.where([=](const T& u) {return u[f] >= val; });
				}
			}
			else if (cmdA == L"根据")
			{
				std::wstring f, op;
				cmdin >> f >> op;
				if (!cmdin.fail() && std::find(field.begin(), field.end(), f) != field.end())
				{
					if (op == L"升序")res.raw.sort([=](const T& u, const T& v) {return u[f] > v[f]; });
					else if (op == L"降序")res.raw.sort([=](const T& u, const T& v) {return u[f] < v[f]; });
				}
			}
		}
		return res;
	}
	auto begin() { return raw.begin(); }
	auto end() { return raw.end(); }
	auto begin() const { return raw.begin(); }
	auto end() const { return raw.end(); }
	int size() const { return raw.size(); }
};