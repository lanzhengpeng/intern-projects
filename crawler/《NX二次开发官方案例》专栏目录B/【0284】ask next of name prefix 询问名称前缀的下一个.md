### 【0284】ask next of name prefix 询问名称前缀的下一个

#### 代码

```cpp
    static tag_t ask_next_of_name_prefix(char *prefix, tag_t part, tag_t object)  
    {  
        int  
            ii;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
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

> 这段代码是用于在NX CAD系统中查询具有特定前缀名称的对象的函数。
>
> 函数主要逻辑如下：
>
> 1. 首先检查输入参数object是否为NULL_TAG，如果是，将输入的名称前缀prefix全部转换为大写。
> 2. 然后，在指定的部件part内循环遍历所有对象，直到找到名称以prefix开头的对象。
> 3. 在循环中，使用UF_OBJ_cycle_all函数遍历下一个对象，并使用UF_OBJ_ask_name获取对象名称。
> 4. 通过strstr函数检查对象名称是否以prefix开头。
> 5. 如果找到符合条件的对象，则返回该对象；如果遍历完所有对象都没有找到，则返回NULL_TAG。
> 6. 函数返回找到的具有指定前缀名称的对象的标签。
>
> 总的来说，这个函数实现了在NX部件中根据名称前缀查询对象的逻辑，可以用于在二次开发中实现特定对象的查找功能。
>
