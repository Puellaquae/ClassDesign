#pragma once
#include <utility>

struct University
{
	std::wstring id, name, address, province, found_date, website;
	University(std::wstring id_,
		std::wstring name_,
		std::wstring address_,
		std::wstring province_,
		std::wstring foundDate,
		std::wstring website_)
		:id(std::move(id_)),
		name(std::move(name_)),
		address(std::move(address_)),
		province(std::move(province_)),
		found_date(std::move(foundDate)),
		website(std::move(website_))
	{}

	explicit University(const std::wstring& rawdata)
	{
		std::wstringstream wss(rawdata);
		wss >> id >> name >> address >> province >> found_date >> website;
	}

	explicit operator std::wstring() const
	{
		std::wstringstream wss;
		wss << id << "\t" << name << "\t" << address << "\t" << province << "\t" << found_date << "\t" << website;
		return wss.str();
	}
	University() = default;
	std::wstring& operator[](const int index)
	{
		switch (index)
		{
		case 0:return id;
		case 1:return name;
		case 2:return address;
		case 3:return province;
		case 4:return found_date;
		case 5:return website;
		}
	}
	std::wstring operator[](const int index) const
	{
		switch (index)
		{
		case 0:return id;
		case 1:return name;
		case 2:return address;
		case 3:return province;
		case 4:return found_date;
		case 5:return website;
		}
	}
	std::wstring& operator[](const std::wstring& index)
	{
		if (index == L"���")return id;
		if (index == L"����")return name;
		if (index == L"��ַ")return address;
		if (index == L"ʡ��")return province;
		if (index == L"��Уʱ��")return found_date;
		if (index == L"��ַ")return website;
	}
	std::wstring operator[](const std::wstring& index) const
	{
		if (index == L"���")return id;
		if (index == L"����")return name;
		if (index == L"��ַ")return address;
		if (index == L"ʡ��")return province;
		if (index == L"��Уʱ��")return found_date;
		if (index == L"��ַ")return website;
	}
	static const std::vector<std::wstring> field;
};
const std::vector<std::wstring> University::field = { L"���",L"����",L"��ַ",L"ʡ��",L"��Уʱ��",L"��ַ" };