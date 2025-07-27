### 【0244】ask kf reference 询问知识框架参考

#### 代码

```cpp
    static char *ask_kf_reference(tag_t object)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *datatype,  
            *name_chain,  
            *string;  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_list_p_t  
            list = NULL;  
        UF_KF_value_p_t  
            sub_list,  
            value;  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_ask_fusion_object(object, &instance));  
        if (instance == NULL)  
        {  
            UF_CALL(UF_KF_make_tag(object, &value));  
            UF_CALL(UF_KF_list_push(list, value, &list));  
            UF_CALL(UF_KF_make_list(list, &value));  
            UF_CALL(UF_KF_ask_value_of_list_item(value, 1, &string, &datatype,  
                &sub_list));  
            UF_free(datatype);  
        }  
        else  
        {  
            UF_CALL(UF_KF_ask_name_chain_of_instance(instance, &name_chain));  
            UF_CALL(allocate_memory(strlen(name_chain)-4, (void **)&string));  
            strcpy(string, strstr(name_chain, ":")+1);  
        }  
        return string;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其主要功能是查询一个NX对象在知识融合(Knowledge Fusion)中的引用名称。
>
> 代码的主要逻辑如下：
>
> 1. 获取当前显示部件的tag。
> 2. 初始化部件的Knowledge Fusion环境。
> 3. 查询传入对象在Knowledge Fusion中的实例。如果实例为空，说明传入对象是根对象，直接查询其引用名称。
> 4. 如果实例不为空，查询实例的名称链，并从中提取出引用名称。
> 5. 返回引用名称字符串。
>
> 这段代码主要用于在Knowledge Fusion中获取一个NX对象的引用名称，通常用于在二次开发中查询和处理Knowledge Fusion数据。
>
