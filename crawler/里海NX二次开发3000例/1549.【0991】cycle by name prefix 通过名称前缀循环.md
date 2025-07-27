### 【0991】cycle by name prefix 通过名称前缀循环

#### 代码

```cpp
    static tag_t cycle_by_name_prefix(char *prefix, tag_t part, tag_t object,  
        char *fullname)  
    {  
        int  
            ii;  
     /*  make sure the name is all in upper case 译:确保名称全部使用大写字母。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要功能是按名称前缀循环查找对象，并返回找到的第一个对象。代码逻辑如下：
>
> 1. 参数说明：prefix: 名称前缀字符串。part: NX部件对象。object: 当前循环到的对象，初始为NULL_TAG。fullname: 临时字符串，用于保存对象的全名。
> 2. prefix: 名称前缀字符串。
> 3. part: NX部件对象。
> 4. object: 当前循环到的对象，初始为NULL_TAG。
> 5. fullname: 临时字符串，用于保存对象的全名。
> 6. 代码流程：首先，将名称前缀字符串转换为大写。然后，循环遍历部件对象part的子对象。在循环中，通过UF_OBJ_cycle_all函数获取下一个对象，直到找到第一个名称以prefix开头的大写字符串。找到后，返回该对象。
> 7. 首先，将名称前缀字符串转换为大写。
> 8. 然后，循环遍历部件对象part的子对象。
> 9. 在循环中，通过UF_OBJ_cycle_all函数获取下一个对象，直到找到第一个名称以prefix开头的大写字符串。
> 10. 找到后，返回该对象。
> 11. 代码功能：该函数的作用是按照名称前缀在NX部件中查找对象，并返回找到的第一个对象。可用于快速定位具有特定前缀的对象。
> 12. 该函数的作用是按照名称前缀在NX部件中查找对象，并返回找到的第一个对象。可用于快速定位具有特定前缀的对象。
> 13. 注意事项：函数内部使用strstr函数检查名称是否以prefix开头。返回的对象为NULL_TAG时表示未找到匹配对象。名称前缀需要全部转换为大写，确保匹配时不区分大小写。
> 14. 函数内部使用strstr函数检查名称是否以prefix开头。
> 15. 返回的对象为NULL_TAG时表示未找到匹配对象。
> 16. 名称前缀需要全部转换为大写，确保匹配时不区分大小写。
> 17. 使用场景：当需要在NX模型中根据名称前缀快速查找对象时，可以使用这个函数。例如查找所有名称以“SHAFT”开头的对象。
> 18. 当需要在NX模型中根据名称前缀快速查找对象时，可以使用这个函数。例如查找所有名称以“SHAFT”开头的对象。
>
