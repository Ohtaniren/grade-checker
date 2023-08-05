#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

/*クラス*/
//授業クラスの定義（科目群，科目種類，授業番号，授業名，単位数，成績評価，取得年度）
struct lecture {
public:
	string category;//科目群
	string lecturekinds;//科目種類
	int lecturenum;//授業番号
	string lecturename;//授業名
	float unit;//単位数
	int value;//成績評価
	int year;//取得年度

	lecture(const string& c, const string& k, int num, const string& name, float u, int v, int y)
		: category(c), lecturekinds(k), lecturenum(num), lecturename(name), unit(u), value(v), year(y)
	{}
};
//3年修了時点での単位数クラスの定義
struct Unitnum3 {
public:
	float unit1;//教養科目群，基盤科目群，キャリア教育の単位数//(3年:14以上，4年:14以上)

	float unit2;//基礎ゼミナール//(3年:2，4年:2)

	float unit3;//情報リテラシー実践//(3年:2，4年:2)

	float unit4;//言語科目（実践英語）//(3年:8，4年:8)

	float unit5;//理系共通基礎科目//(3年:20以上，4年:20以上)

	float unit6;//コース共通基礎科目（必修）：//(3年:16)

	float unit7;//コース共通基礎科目（選択）：//(3年:10以上)

	float unit8;//コース共通専門，コース専門科目群：//(32単位以上＊コース専門科目群：所属コース（知能 / 生体）の科目に限る)

	float unit9;//知能生体応用実験//(3年:2，4年:2)

	float unit10;//専門教育科目群//(3年:62)

	float unit11;//未修言語科目，保健体育科目，理系共通基礎科目，専門教育科目群：(3年:86以上,4年:104以上)

	float unit12;//総単位//(3年:112以上，4年:130以上)//

	Unitnum3(float u1, float u2, float u3, float u4, float u5, float u6, float u7, float u8, float u9, float u10, float u11, float u12)
		: unit1(u1), unit2(u2), unit3(u3), unit4(u4), unit5(u5), unit6(u6), unit7(u7), unit8(u8), unit9(u9), unit10(u10), unit11(u11), unit12(u12)
	{}
};
//4年修了時点での単位数クラスの定義
struct Unitnum4 {
public:
	float unit1;//コース共通基礎など//(4年:66以上)

	float unit2;//特別研究//(4年:8)

	float unit3;//専門教育科目群//(4年:80)

	float unit4;//未修言語科目，保健体育科目，理系共通基礎科目，専門教育科目群//(4年:104以上)

	float unit5;//総単位//(4年:130以上)

	Unitnum4(float u1, float u2, float u3, float u4, float u5)
		: unit1(u1), unit2(u2), unit3(u3), unit4(u4), unit5(u5)
	{}
};

/*関数*/
//文字をスペースで区切り，区切られた文字をクラスに格納する関数（イテレータの使用）
void splitString(const string& line, vector<string>& elements) {
	// 半角スペースを全角スペースに置換する（教員名の空白の統一のため）
	string replacedLine = line;
	for (char& c : replacedLine) {
		if (c == ' ' ) { 
			c = '　';
		}
	}

	regex re("\\s+"); // 正規表現パターン: 1つ以上の空白文字（全角スペース）にマッチ
	sregex_token_iterator it(replacedLine.begin(), replacedLine.end(), re, -1); // イテレータの使用
	sregex_token_iterator end;
	while (it != end) {
		elements.push_back(*it++);
	}
}

//秀を4、優を3、良を2、可を1、不可を0の数値に変換する関数
int convertGradeToValue(const string& grade) {
	if (grade == "秀") return 4;
	else if (grade == "優") return 3;
	else if (grade == "良") return 2;
	else if (grade == "可") return 1;
	else if (grade == "不可")return 0;
	else return -1;// 未定義の成績が渡された場合のデフォルト値
}

//単位数計算関数
float unitsum(const vector<lecture>& lectures, const string& category, const string& kinds) {
	float unitSum = 0.0;
	for (const auto& lec : lectures) {
		if (lec.category == category && lec.lecturekinds == kinds) {
			if (lec.value != 0) {
				unitSum += lec.unit;
			}
		}
	}
	return unitSum;
}
//単位数計算関数(part2)
float unitsum2(const vector<lecture>& lectures, const string& category, const string& kinds, const string& lecturename) {
	float unitSum = 0.0;
	for (const auto& lec : lectures) {
		if (lec.category == category && lec.lecturekinds == kinds && lec.lecturename == lecturename) {
			if (lec.value != 0) {
				unitSum += lec.unit;
			}
		}
	}
	return unitSum;
}
//単位数計算関数(part3)
float unitsum3(const vector<lecture>& lectures, const string& category, const string& kinds, const string& lecturename, int lecturenum) {
	float unitSum = 0.0;
	for (const auto& lec : lectures) {
		if (lec.category == category && lec.lecturekinds == kinds && lec.lecturename == lecturename && lec.lecturenum == lecturenum) {
			if (lec.value != 0) {
				unitSum += lec.unit;
			}
		}
	}
	return unitSum;
}
//単位数計算関数(part4)
float unitsum4(const vector<lecture>& lectures, const string& category, const string& kinds, int lecturenum) {
	float unitSum = 0.0;
	for (const auto& lec : lectures) {
		if (lec.category == category && lec.lecturekinds == kinds  && lec.lecturenum == lecturenum) {
			if (lec.value != 0) {
				unitSum += lec.unit;
			}
		}
	}
	return unitSum;
}

// 取得単位数と必要単位数を比較する関数(3年次修了判定)
void compareUnits(const Unitnum3& acquired, const Unitnum3& required) {
	cout << "【3年次修了判定】" << endl;
	// 各要素の比較を行い、結果に応じてメッセージを表示します
	if (acquired.unit1 >= required.unit1)
		cout << "基礎科目群+基盤科目群+キャリア教育の単位数 (" << acquired.unit1 << ") は十分です。" << endl;
	else
		cout << "基礎科目群+基盤科目群+キャリア教育の単位数 (" << acquired.unit1 << ") が不足しています。\n必要な単位数: " << required.unit1 << endl;

	if (acquired.unit2 >= required.unit2)
		cout << "基礎ゼミナールの単位数 (" << acquired.unit2 << ") は十分です。" << endl;
	else
		cout << "基礎ゼミナールの単位数 (" << acquired.unit2 << ") が不足しています。\n必要な単位数: " << required.unit2 << endl;

	// 各要素の比較を追加します
	if (acquired.unit3 >= required.unit3)
		cout << "情報リテラシー実践Ⅰの単位数 (" << acquired.unit3 << ") は十分です。" << endl;
	else
		cout << "情報リテラシー実践Ⅰの単位数 (" << acquired.unit3 << ") が不足しています。\n必要な単位数: " << required.unit3 << endl;

	if (acquired.unit4 >= required.unit4)
		cout << "実践英語の単位数 (" << acquired.unit4 << ") は十分です。" << endl;
	else
		cout << "実践英語の単位数 (" << acquired.unit4 << ") が不足しています。\n必要な単位数: " << required.unit4 << endl;

	if (acquired.unit5 >= required.unit5)
		cout << "理系共通基礎科目の単位数 (" << acquired.unit5 << ") は十分です。" << endl;
	else
		cout << "理系共通基礎科目の単位数 (" << acquired.unit5 << ") が不足しています。\n必要な単位数: " << required.unit5 << endl;

	if (acquired.unit6 >= required.unit6)
		cout << "コース共通基礎科目（必修）の単位数 (" << acquired.unit6 << ") は十分です。" << endl;
	else
		cout << "コース共通基礎科目（必修）の単位数 (" << acquired.unit6 << ") が不足しています。\n必要な単位数: " << required.unit6 << endl;

	if (acquired.unit7 >= required.unit7)
		cout << "コース共通基礎科目（選択）の単位数 (" << acquired.unit7 << ") は十分です。" << endl;
	else
		cout << "コース共通基礎科目（選択）の単位数 (" << acquired.unit7 << ") が不足しています。\n必要な単位数: " << required.unit7 << endl;

	if (acquired.unit8 >= required.unit8)
		cout << "コース共通専門科目+コース専門科目の単位数 (" << acquired.unit8 << ") は十分です。" << endl;
	else
		cout << "コース共通専門科目+コース専門科目の単位数 (" << acquired.unit8 << ") が不足しています。\n必要な単位数: " << required.unit8 << endl;

	if (acquired.unit9 >= required.unit9)
		cout << "知能・生体機械応用実験の単位数 (" << acquired.unit9 << ") は十分です。" << endl;
	else
		cout << "知能・生体機械応用実験の単位数 (" << acquired.unit9 << ") が不足しています。\n必要な単位数: " << required.unit9 << endl;

	if (acquired.unit10 >= required.unit10)
		cout << "専門教育科目群の単位数 (" << acquired.unit10 << ") は十分です。" << endl;
	else
		cout << "専門教育科目群の単位数 (" << acquired.unit10 << ") が不足しています。\n必要な単位数: " << required.unit10 << endl;

	if (acquired.unit11 >= required.unit11)
		cout << "未修言語科目+保健体育科目+理系共通基礎科目+専門教育科目群の単位数 (" << acquired.unit11 << ") は十分です。" << endl;
	else
		cout << "未修言語科目+保健体育科目+理系共通基礎科目+専門教育科目群の単位数 (" << acquired.unit11 << ") が不足しています。\n必要な単位数: " << required.unit11 << endl;

	if (acquired.unit12 >= required.unit12)
		cout << "総単位数 (" << acquired.unit12 << ") は十分です。" << endl;
	else
		cout << "総単位数 (" << acquired.unit12 << ") が不足しています。\n必要な単位数: " << required.unit12 << endl;

	// すべての要素の比較結果に応じた分岐を追加します
	if (acquired.unit1 >= required.unit1 && acquired.unit2 >= required.unit2 && acquired.unit3 >= required.unit3
		&& acquired.unit4 >= required.unit4 && acquired.unit5 >= required.unit5 && acquired.unit6 >= required.unit6
		&& acquired.unit7 >= required.unit7 && acquired.unit8 >= required.unit8 && acquired.unit9 >= required.unit9
		&& acquired.unit10 >= required.unit10 && acquired.unit11 >= required.unit11 && acquired.unit12 >= required.unit12){
		// 条件がすべて満たされた場合の処理
		cout << "→［3年次修了要件を満たしています］\n" << endl;
	}
	else{
		// 条件が満たされなかった場合の処理
		cout << "→［3年次修了要件を満たしていません］\n" << endl;
	}
}
// 取得単位数と必要単位数を比較する関数(卒業次修了判定)
void compareUnits2(const Unitnum4& acquired, const Unitnum4& required) {
cout << "【卒業判定】" << endl;
// 各要素の比較を行い、結果に応じてメッセージを表示します
if (acquired.unit1 >= required.unit1)
cout << "コース共通基礎科目+コース共通専門科目+コース専門科目の単位数 (" << acquired.unit1 << ") は十分です。" << endl;
else
cout << "コース共通基礎科目+コース共通専門科目+コース専門科目の単位数 (" << acquired.unit1 << ") が不足しています。\n必要な単位数: " << required.unit1 << endl;

if (acquired.unit2 >= required.unit2)
cout << "知能・生体機械特別研究の単位数 (" << acquired.unit2 << ") は十分です。" << endl;
else
cout << "知能・生体機械特別研究の単位数 (" << acquired.unit2 << ") が不足しています。\n必要な単位数: " << required.unit2 << endl;

// 各要素の比較を追加します
if (acquired.unit3 >= required.unit3)
cout << "専門教育科目群の単位数 (" << acquired.unit3 << ") は十分です。" << endl;
else
cout << "専門教育科目群の単位数 (" << acquired.unit3 << ") が不足しています。\n必要な単位数: " << required.unit3 << endl;

if (acquired.unit4 >= required.unit4)
cout << "未修言語科目+保健体育科目+理系共通基礎科目+専門教育科目群の単位数 (" << acquired.unit4 << ") は十分です。" << endl;
else
cout << "未修言語科目+保健体育科目+理系共通基礎科目+専門教育科目群の単位数 (" << acquired.unit4 << ") が不足しています。\n必要な単位数: " << required.unit4 << endl;

if (acquired.unit5 >= required.unit5)
cout << "総単位数 (" << acquired.unit5 << ") は十分です。" << endl;
else
cout << "総単位数 (" << acquired.unit5 << ") が不足しています。\n必要な単位数: " << required.unit5 << endl;


// すべての要素の比較結果に応じた分岐を追加します
if (acquired.unit1 >= required.unit1 && acquired.unit2 >= required.unit2 && acquired.unit3 >= required.unit3
	&& acquired.unit4 >= required.unit4 && acquired.unit5 >= required.unit5 ){
	// 条件がすべて満たされた場合の処理
	cout << "→［卒業要件を満たしています］\n" << endl;
}
else{
	// 条件が満たされなかった場合の処理
	cout << "→［卒業要件を満たしていません］\n" << endl;
}
}

//gpa 計算関数
float calculateGPA(const vector<lecture>& lectures) {
	float totalGrade = 0.0;
	float totalUnits = 0.0;

	for (const auto& lec : lectures) {
		totalGrade += lec.value * lec.unit;
		totalUnits += lec.unit;
	}

	if (totalUnits > 0) {
		return totalGrade / totalUnits;
	}
	else {
		return 0.0;
	}
}



//メイン関数
int main() {
	//操作の手順の説明
	cout << "～～使い方～～" << endl;
	cout << "【１】所属コースを選択してください" << endl;
	cout << "【２】『CAMPUSSQUAREにログイン』 → 『成績』 → 『過去を含めた全成績を選択』 → 『画面に表示するをクリック』" << endl;
	cout << "の手順で成績を表示し，No.1から最後まで，表のすべての項目をコピーしたものを以下に貼り付けてください" << endl;
	cout << "※テキスト貼り付け時，警告が出ることがありますが，「強制的に貼り付け」をクリックして問題ありません．\n" << endl ;
	
	//コースの選択
	cout << "【１】あなたの所属コースを選択してください（知能機械コース: 1, 生体機械コース: 2）: ";
	int courseSelection;
	cin >> courseSelection;
	cout << "選択されたコース: " << courseSelection << endl <<endl;


	/*入力テキストのファイルの保存*/
	ofstream outfile("seiseki.txt");
	//ファイル入力失敗時のメッセージ
	if (!outfile) {
		cout << "ファイルの作成に失敗しました。" << endl;
		return 1;
	}

	cout << "【２】以下に指定したテキストをペーストしてください（終了するには改行後，Ctrl+Zを押してからenterを押してください）:" << endl;
	string line2;
	//各列ごとにファイル保存
	while (getline(cin, line2)) {
		outfile << line2 << endl;
	}
	outfile.close();
	cout << "ファイルの保存が完了しました" << endl;


	/*入力してもらったtxtファイルの読み込み*/
	ifstream infile("seiseki.txt");
	vector<lecture>lectures;//lecture型のvector（名前はlectures）
	string line;
	vector<string> elements;

	while (getline(infile, line)) {

		splitString(line, elements);//関数の利用

		//11つに分かれるとき
		if (elements.size() == 11) {
			string category = elements[1];
			string kinds = elements[2];
			string num = elements[3];
			string name = elements[4];
			float unit = stof(elements[6]);//文字列をfloatに変換
			string grade = elements[9];
			int value = convertGradeToValue(grade);//漢字を数値に変換
			int year = stoi(elements[7]);

			// numを1文字目と2文字目以降に分割して保存（数値のみ保存させる）
			string alphaPart = num.substr(0, 1);
			int numericPart = stoi(num.substr(1));

			lecture lec(category, kinds, numericPart, name, unit, value, year);
			lectures.push_back(lec);
		}
		//12つに分かれるとき
		if (elements.size() == 12) {
			string category = elements[1];
			string kinds = elements[2];
			string num = elements[4];
			string name = elements[5];
			float unit = stof(elements[7]);//文字列をfloatに変換
			string grade = elements[10];
			int value = convertGradeToValue(grade);
			int year = stoi(elements[8]);

			// numを1文字目と2文字目以降に分割して保存
			string alphaPart = num.substr(0, 1);
			int numericPart = stoi(num.substr(1));

			lecture lec(category, kinds, numericPart, name, unit, value, year);
			lectures.push_back(lec);
		}
		elements.clear();
	}
	infile.close();
	cout << "ファイルが確認できました\n" << endl;

	//各区分けに置ける単位数の計算（システムデザイン学部）
	//u1:基礎科目群+基盤科目群+キャリア教育
	float u1_1 = unitsum(lectures, "教養科目", "都市・社会・環境");
	float u1_2 = unitsum(lectures, "教養科目", "科学・技術・産業");
	float u1_3 = unitsum(lectures, "基盤科目", "社会科学領域");
	float u1_4 = unitsum(lectures, "基盤科目", "自然科学領域");
	float u1_5 = unitsum(lectures, "基礎科目", "キャリア教育");
	float u1 = u1_1+ u1_2 + u1_3 + u1_4 + u1_5 ;

	//u2:基礎ゼミナール
	float u2 = unitsum(lectures, "基礎科目", "基礎ゼミナール");

	//u3:情報リテラシー実践Ⅰ
	float u3 = unitsum(lectures, "基礎科目", "情報科目");

	//u4:実践英語
	float u4 = unitsum(lectures, "基礎科目", "実践英語");

	//u5:理系共通基礎科目
	float u5 = unitsum(lectures, "基礎科目", "理系共通基礎科目"); 

	//u6:コース共通基礎科目（必修）
	float u6_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "工業力学");
	float u6_2 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",417);//材料力学Ⅰ
	float u6_3 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",413);//機械製図Ⅰ
	float u6_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "工業数学");
	float u6_5 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機構学");
	float u6_6 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 422);//機械製図Ⅱ
	float u6_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機械システム基礎実験");
	float u6_8 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機械工作実習");
	float u6 = u6_1 + u6_2 + u6_3 + u6_4 + u6_5 + u6_6 + u6_7 + u6_8;

	//u7:コース共通基礎科目（選択）
	float u7_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "計測工学");
	float u7_2 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 407);//プログラミング演習Ⅰ
	float u7_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "基礎電気回路");
	float u7_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "メカトロニクス@(Mechatronics)");
	float u7_5 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 416);//機械力学Ⅰ
	float u7_6 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 417);//流体力学Ⅰ
	float u7_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "熱力学");
	float u7_8 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 426);//システム制御Ⅰ
	float u7_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "電子回路");
	float u7 = u7_1 + u7_2 + u7_3 + u7_4 + u7_5 + u7_6 + u7_7 + u7_8 + u7_9;

	//u8:コース共通専門科目，コース専門科目
	//コース共通専門科目
	float u8_1_1 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 419);//材料力学Ⅱ
	float u8_1_2 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 428);//機械力学Ⅱ
	float u8_1_3 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 429);//流体力学Ⅱ
	float u8_1_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "熱工学");
	float u8_1_5 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機械要素");
	float u8_1_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "基礎加工学");
	float u8_1_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機械材料学");
	float u8_1_8 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 437);//機械工学演習Ⅰ
	float u8_1_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "機械システム先端講義");
	float u8_1_10 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "応用加工学");
	float u8_1_11 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "マイクロ・ナノ工学");
	float u8_1_12 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",456);//機械工学演習Ⅱ
	float u8_1_13 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "インターンシップ");
	float u8_1_14 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "工場見学");
	float u8_1 = u8_1_1 + u8_1_2 + u8_1_3 + u8_1_4 + u8_1_5 + u8_1_6 + u8_1_7 + u8_1_8 + u8_1_9 + u8_1_10 + u8_1_11+ u8_1_12 + u8_1_13 + u8_1_14;
	//コース専門科目
	float u8_2 = 0; 
	if (courseSelection == 1) {//知能
		float u8_2_0 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械応用実験");
		float u8_2_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械概論");
		float u8_2_2 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム解析");
		float u8_2_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "ロボット運動学");
		float u8_2_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能ロボット");
		float u8_2_5 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 441); //創造的ロボティクス演習Ⅰ
		float u8_2_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム工学");
		float u8_2_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "基礎設計工学");
		float u8_2_8 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 444);//システム制御Ⅱ
		float u8_2_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム制御演習");
		float u8_2_10 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 446);//プログラミング演習Ⅱ
		float u8_2_11 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械ゼミナール");
		float u8_2_12 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "組込みシステム");
		float u8_2_13 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "ロボットセンシング");
		float u8_2_14 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "応用設計工学");
		float u8_2_15 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム工学演習");
		float u8_2_16 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械特別研究");
		float u8_2_17 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 471);//創造的ロボティクス演習Ⅱ
		u8_2 = u8_2_0 + u8_2_1 + u8_2_2 + u8_2_3 + u8_2_4 + u8_2_5 + u8_2_6 + u8_2_7 + u8_2_8 + u8_2_9 + u8_2_10 + u8_2_11 + u8_2_12 + u8_2_13 + u8_2_14 + u8_2_15 + u8_2_16 + u8_2_17;
	}
	else if (courseSelection == 2) {//生体
		float u8_2_0 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械応用実験");
		float u8_2_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械概論");
		float u8_2_2 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体生理工学");
		float u8_2_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体統計学");
		float u8_2_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "バイオメカニクス");
		float u8_2_5 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生理流体工学");
		float u8_2_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "福祉機器工学"); 
		float u8_2_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体計測工学");
		float u8_2_8 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "エネルギー変換工学");
		float u8_2_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体材料強度工学");
		float u8_2_10 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 453);//生体機械工学演習Ⅰ
		float u8_2_11 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械ゼミナール");
		float u8_2_12 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "再生医工学");
		float u8_2_13 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "人間工学");
		float u8_2_14 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "数値解析法");
		float u8_2_15 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 466);//生体機械工学演習Ⅱ
		float u8_2_16 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械特別研究");
		float u8_2_17 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生命理論");
		u8_2 = u8_2_0 + u8_2_1 + u8_2_2 + u8_2_3 + u8_2_4 + u8_2_5 + u8_2_6 + u8_2_7 + u8_2_8 + u8_2_9 + u8_2_10 + u8_2_11 + u8_2_12 + u8_2_13 + u8_2_14 + u8_2_15 + u8_2_16 + u8_2_17;
	}
	 float u8 = u8_1 + u8_2;

	//u9:知能・生体機械応用実験
	float u9=0;
	if (courseSelection == 1) {
		u9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械応用実験");
	}
	else if (courseSelection == 2) {
		u9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械応用実験");
	}

	//u10:専門教育科目群
	//学部共通科目
	float u10_1_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "情報と職業");
	float u10_1_2 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "科学技術英語第一");
	float u10_1_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "科学技術英語第二");
	float u10_1_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システムデザイン論");
	float u10_1_5 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "経営工学概論");
	float u10_1_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "工学倫理");
	float u10_1_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "産業と法規");
	float u10_1 = u10_1_1 + u10_1_2 + u10_1_3 + u10_1_4 + u10_1_5 + u10_1_6 + u10_1_7;
	//所属コースでない科目
	float u10_2 =0;
	if (courseSelection == 2) {//生体
		float u10_2_0 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械応用実験");
		float u10_2_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械概論");
		float u10_2_2 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム解析");
		float u10_2_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "ロボット運動学");
		float u10_2_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能ロボット");
		float u10_2_5 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",441); //創造的ロボティクス演習Ⅰ
		float u10_2_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム工学");
		float u10_2_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "基礎設計工学");
		float u10_2_8 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",444);//システム制御Ⅱ
		float u10_2_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム制御演習");
		float u10_2_10 = unitsum4(lectures, "専門教育科目", "システムデザイン学部",446);//プログラミング演習Ⅱ
		float u10_2_11 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械ゼミナール");
		float u10_2_12 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "組込みシステム");
		float u10_2_13 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "ロボットセンシング");
		float u10_2_14 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "応用設計工学");
		float u10_2_15 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "システム工学演習");
		float u10_2_16 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械特別研究");
		float u10_2_17 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 471);//創造的ロボティクス演習Ⅱ

		u10_2 = u10_2_0 + u10_2_1 + u10_2_2 + u10_2_3 + u10_2_4 + u10_2_5 + u10_2_5 + u10_2_6 + u10_2_7 + u10_2_8 + u10_2_9 + u10_2_10 + u10_2_11 + u10_2_12 + u10_2_13 + u10_2_14 + u10_2_15 + u10_2_16 + u10_2_17;
	}
	else if (courseSelection == 1) {//知能
		float u10_2_0 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械応用実験");
		float u10_2_1 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械概論");
		float u10_2_2 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体生理工学");
		float u10_2_3 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体統計学");
		float u10_2_4 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "バイオメカニクス");
		float u10_2_5 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生理流体工学");
		float u10_2_6 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "福祉機器工学");
		float u10_2_7 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体計測工学");
		float u10_2_8 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "エネルギー変換工学");
		float u10_2_9 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体材料強度工学");
		float u10_2_10 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 453);//生体機械工学演習Ⅰ
		float u10_2_11 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械ゼミナール");
		float u10_2_12 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "再生医工学");
		float u10_2_13 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "人間工学");
		float u10_2_14 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "数値解析法");
		float u10_2_15 = unitsum4(lectures, "専門教育科目", "システムデザイン学部", 466);//生体機械工学演習Ⅱ
		float u10_2_16 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械特別研究");
		float u10_2_17 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生命理論");

		u10_2 = u10_2_0 + u10_2_1 + u10_2_2 + u10_2_3 + u10_2_4 + u10_2_5 + u10_2_6 + u10_2_7 + u10_2_8 + u10_2_9 + u10_2_10 + u10_2_11 + u10_2_12 + u10_2_13 + u10_2_14 + u10_2_15 + u10_2_16 + u10_2_17;
	}
	//学部外科目と副専攻
	float u10_3_1 = unitsum(lectures, "専門教育科目", "人文社会学部");
	float u10_3_2 = unitsum(lectures, "専門教育科目", "法学部");
	float u10_3_3 = unitsum(lectures, "専門教育科目", "経済経営学部");
	float u10_3_4 = unitsum(lectures, "専門教育科目", "理学部");
	float u10_3_5 = unitsum(lectures, "専門教育科目", "都市環境学部");
	float u10_3_6 = unitsum(lectures, "専門教育科目", "健康福祉学部");
	float u10_3_7 = unitsum(lectures, "専門教育科目", "副専攻");
	float u10_3 = u10_3_1 + u10_3_2 + u10_3_3 + u10_3_4 + u10_3_5 + u10_3_6 + u10_3_7;
	float u10 = u6 + u7 + u8 + u10_1 + u10_2 + u10_3;

	//u11:未修言語科目，保健体育科目，理系共通基礎科目，専門教育科目群
	float u11_1 = unitsum(lectures, "基礎科目", "未修言語");
	float u11_2 = unitsum(lectures, "基礎科目", "保険体育");
	float u11 = u10 + u5 + u11_1 + u11_2;

	//u12:総単位
	float u12 = u11 + u1 + u2 + u3 + u4;

	//取得単位数の格納
	vector<Unitnum3>num3;
	Unitnum3 num(u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12);
	num3.push_back(num);
	//3年修了時の必要単位数の格納
	Unitnum3 nnum(14, 2, 2, 8, 20, 16, 10, 32, 2, 62, 86, 112);
	num3.push_back(nnum);
	//取得単位数と3年修了時の必要単位数の比較
	compareUnits(num3[0], num3[1]);


	/*卒業要件(3年修了判定を満たしているとき)*/
	//u21:コース共通基礎科目,コース共通専門科目,コース専門科目
	float u21 = u6 + u7 + u8;

	//u22:知能・生体機械特別研究
	float u22=0;
	if (courseSelection == 1) {
		u22 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "知能機械特別研究");
	}
	else if (courseSelection == 2) {
		u22 = unitsum2(lectures, "専門教育科目", "システムデザイン学部", "生体機械特別研究");
	}

	//u23:専門教育科目群
	float u23 = u10;

	//u24:未修言語科目，保健体育科目，理系共通基礎科目，専門教育科目群
	float u24 = u11;

	//u25:総単位
	float u25 = u12;

	//取得単位数の格納
	vector<Unitnum4>num4;
	Unitnum4 num2(u21, u22, u23, u24, u25);
	num4.push_back(num2);
	//必要単位数の格納
	Unitnum4 nnum2(66, 8, 80, 104, 130);
	num4.push_back(nnum2);
	//必要単位数と取得単位数の比較
	compareUnits2(num4[0], num4[1]);


	//GPAの計算
	float gpa = calculateGPA(lectures);
	cout << "取得単位のGPA（Grade Point Average）は: " << gpa << "です。" << endl;
	

	return 0; 
}
