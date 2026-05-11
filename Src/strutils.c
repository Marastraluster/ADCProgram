/**
  * @brief  比较两个字符串是否相同
  * @param  s1 字符串1
  * @param  s2 字符串2
  * @retval 1=相同，0=不同
  */
unsigned char Str_Compare(unsigned char *s1, unsigned char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2) return 0;
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

/**
  * @brief  从字符串中解析十进制数字
  * @param  Str 指向数字起始位置的指针
  * @retval 解析出的数值
  */
unsigned int Str_GetNumber(unsigned char *Str)
{
	unsigned int Result = 0;
	while (*Str >= '0' && *Str <= '9')
	{
		Result = Result * 10 + (*Str - '0');
		Str++;
	}
	return Result;
}
