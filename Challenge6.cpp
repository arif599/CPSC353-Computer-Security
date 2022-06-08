
// C++ program to find hamming distance b/w
// two string
#include <string>
#include <map>
#include <vector>

using namespace std;
 
map<char, string> gen_hex_table()
{
	map<char, string> map;

	map['0'] = "0000";
	map['1'] = "0001";
	map['2'] = "0010";
	map['3'] = "0011";
	map['4'] = "0100";
	map['5'] = "0101";
	map['6'] = "0110";
	map['7'] = "0111";
	map['8'] = "1000";
	map['9'] = "1001";
	map['a'] = "1010";
	map['b'] = "1011";
	map['c'] = "1100";
	map['d'] = "1101";
	map['e'] = "1110";
	map['f'] = "1111";

	return map;
}
int hamming_distance(string str1, string str2)
{
  int count=0;
  for(int i=0; i<str1.size(); i++)
  {
    int partial = (str1[i]) ^ (str2[i]);
    while(partial)
    {
      count += partial & 1;
      partial = partial >> 1;
    }
  }
  return count;
}
vector<int> con_bin_2_dec(string str, double power)
{
	vector<int> v;
	string newStr = "";
	istringstream iss(str);
	string x;

	while(iss >> x)
	{
		double p = power;
		double decimal = 0.0;

		for(int i=0; i<x.size(); i++)
		{
			if(x[i] == '1')
			{
				decimal += pow(2.0, p);
			}
			p--;
		}
		v.push_back((int)decimal);
	}
	return v;
}


string add_spaces(string str, int spaces)
{
	string newStr = "";
	int count = 0;

	for(int i=0; i<str.size(); i++)
	{

		// newStr += str[i];
		if(count == spaces)
		{
			newStr += " ";
			i--;
			count = 0;
		}
		else
		{
			newStr += str[i];
			count++;
		}
	}

	return newStr;
}
string con_hex_2_bin(string hexStr)
{
  cout << hexStr << endl;
	map<char,string> m = gen_hex_table();

	string newStr = "";
	for(int i=0; i<hexStr.size(); i++)
	{
		if(isdigit(hexStr[i]))
		{
			newStr += m.find(hexStr[i])->second;
		}
		else
		{
			newStr += m.find(hexStr[i])->second;
		}
		// newStr += m.find(hexStr[i])->second;
	}
  cout << "In con_hex_2_bin: " << newStr << endl;
	return newStr;
}
string con_hex_2_ascii(string str)
{
	string newStr = "";
	str = add_spaces(con_hex_2_bin(str), 8);
  cout << str << endl;
	vector<int> v = con_bin_2_dec(str, 7.0);

	for(int i=0; i<v.size(); i++)
	{
		newStr += (char)v[i];
	}
  
	return newStr;
}

// string repeatingKeyXOR(string str, string key)
// {
//   string newStr = "";
//   int count = 0;

//   for(int i = 0; i<str.size(); i++)
//   {
//     unsigned char a = key[count];
//     unsigned char b = str[i];
//     unsigned char c = b ^ a;
//     newStr += c;

//     if(count == key.size() - 1)
//     {
//       count = 0;
        
//     }
//     else
//     {
//       count ++;
//     }
//   }
//   string final = "0";
//   final += asciiToHex(newStr);
//   return final;
// }

// string singleByteXOR_V2(string str, char key)
// {
//   string newStr(str.size(),' '); 

//   for(int i=0; i<str.size(); ++i){
//     newStr[i] = str[i] ^ key;
//   }
//   return newStr;
// }


// char singleByteXOR_Bruteforce_key(string cipherBlock)
// {
//   char key = 0;
//   int maxCount=0;
//   string decodedMessage;

//   for(int i=0; i<=256; i++)
//   {
//     char ch = (char) i;
//     string attempt = singleByteXOR_V2(cipherBlock, ch);

//     int count = 0;
//     for(int j=0; j<attempt.size(); j++)
//     {
//       if((attempt[j] >= 65 && attempt[j] <= 122) || attempt[j] == ' ')
//       {
//         count++;
//       }
//     }
//     if(count > maxCount)
//     {
//       maxCount = count;
//       key = ch;
//     }
//   }


//   return key;
// }

//     //Guess Key length of the cipher
// int guess_key_length(string cipherText)
// {
//   int KEYSIZE = -1;
//   double leastNormalized = INT_MAX;

//   for(int i=2; i<=40; i++)
//   {
//     double normalize = 0.0f;

//     int bytes = cipherText.size()/i;

//     for(int j=0; j<bytes; j++)
//     {
//       string blockA = cipherText.substr((j*i), i);
//       string blockB = cipherText.substr(((j+1)*i), i);
//       int edit_distance = hamming_distance(blockA, blockB);
//       normalize += ((double)edit_distance)/((double)blockA.size());
//     }
//     normalize /= bytes;
//     if(normalize > 0 && normalize < leastNormalized)
//     {
//       leastNormalized = normalize;
//       KEYSIZE = i;
//     }
//   }
//   return KEYSIZE;
// }
    
int main()
{
  char str1[] = "this is a test";
  char str2[] = "wokka wokka!!!";
 
  cout << hamming_distance (str1, str2);
  string message;
  string binary;
  string key;
  
  ifstream infile;

  infile.open("message.txt");
  getline(infile, message, ' ');
  infile.close();
  message = con_hex_2_ascii(message);
  cout << message << endl;

  // int keyLength = guess_key_length(message);

  // int blocks = message.size() / keyLength;

  // for(int i=0; i<keyLength; ++i)
  // {
  //   string block;
  //   char indexKey=' ';
    
  //   for(int j=0; j<blocks; j++){
  //     block += message.substr((j*keyLength) + i,1);
  //   }
  //     key += singleByteXOR_Bruteforce_key(block);
  // }

  // cout << "KEY: " << key << endl;
  // cout << "DECODED: " << con_hex_2_ascii(repeatingKeyXOR(message, key)) << endl;
  
return 0;
}
