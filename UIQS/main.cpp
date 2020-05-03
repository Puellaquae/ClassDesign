#include <string>
#include "Interact.hpp"
#include "DataBase.hpp"
#include "University.hpp"

DataBase<University> dataBase = DataBase<University>(L"data.txt");

int main()
{
	using namespace Interact;
	SetChineseEnvironment();
	typedef Menu::Item item;
	Welcome(L"��У��Ϣ��ѯϵͳ")();

	Menu(L"��ʼ", {
		item(1, L"��Ϣ��ѯ", [] {
			Menu(L"��Ϣ��ѯ", {
				item(1, L"��ʾ���и�У��Ϣ",[] {[&] {
					Table<DataBase<University>>(dataBase.copy())();
					}();
				}),
				item(2, L"ͨ����У[���|����|��ַ|ʡ��|��Уʱ��|��ַ]��ѯ",[] {
					Input<std::wstring>(L"������Ҫ���ڲ��ҵ���Ŀ",[&](const std::wstring& item) {
						if (std::find(University::field.begin(),University::field.end(),item) == University::field.end())
						{
							Output(L"�������")();
							return;
						}
						Input<std::wstring>(L"��������ҵ�ֵ", [&](const std::wstring& val) {
							Table<DataBase<University>>(dataBase.where([&](const University& x) {
								return x[item].find(val) != x[item].npos;
							}))();
						})();
					})();
				}),
				item(3, L"�߼���ѯ",[]() {
					Output(L"��ѯ����˵������Ҫʡ�Կո�:\n\t׼ȷƥ��:\"ѡ�� ��Ŀ�� ���� ֵ\"\t ģ��ƥ��:\"ѡ�� ��Ŀ�� ���� ֵ\"\n\t����:\"���� ��Ŀ�� ����\"\t\"���� ��Ŀ�� ����\"")();
					Input<std::wstring>(L"��ʹ�������ѯ",[&](const std::wstring cmd) {
						Table<DataBase<University>>(dataBase.query(cmd))();
					})();
				}),
				item(4, L"�����ϼ��˵�")
			})();
		}),
		item(2, L"��Ϣά��", [] {
			Menu(L"��Ϣά��", {
				item(1, L"ɾ����Ϣ",[] {
					Input<std::wstring>(L"������Ҫɾ���ĸ�У���",[&](const std::wstring& index) {
						const int res = dataBase.remove([&](const University& u) {return u.id == index; });
						Output(L"���¹�Ӱ��#����Ϣ",res)();
					})();
				}),
				item(2, L"�޸���Ϣ",[] {
					Input<std::wstring>(L"������Ҫ�޸ĵĸ�У���",[](const std::wstring& index) {
						Input<std::wstring>(L"������Ҫ�޸ĵ���Ŀ",[&](const std::wstring& item) {
							if (std::find(University::field.begin(),University::field.end(),item) == University::field.end())
							{
								Output(L"�������")();
								return;
							}
							Input<std::wstring>(L"��������ֵ",[&](const std::wstring& newval) {
								const int res = dataBase.update(item,newval,[&](const University& u) {
									return u.id == index;
								});
								Output(L"���¹�Ӱ��#����Ϣ",res)();
							})();
						})();
					})();
				}),
				item(3, L"������Ϣ",[] {
					Form<University>([&](const University& val) {
						dataBase.insert(val);
						Output(L"��Ϣ�����")();
					})();
				}),
				item(4, L"�����ϼ��˵�")
			})();
		}),
		item(3, L"�˳�ϵͳ")
		})();
		return 0;
}