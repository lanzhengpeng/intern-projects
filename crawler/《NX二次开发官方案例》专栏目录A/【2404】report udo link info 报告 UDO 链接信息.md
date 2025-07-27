### 【2404】report udo link info 报告 UDO 链接信息

#### 代码

```cpp
    static void report_udo_link_info(tag_t udo)  
    {  
        int  
            ii,  
            n,  
            status,  
            subtype,  
            type;  
        char  
            out[UF_UI_MAX_STRING_LEN];  
        UF_UDOBJ_link_t  
            *link_defs;  
        UF_CALL(UF_UI_open_listing_window());  
        n = ask_udo_links(udo, &link_defs);  
        sprintf(out, "UDO %d has %d links\n", udo, n);  
        UF_CALL(UF_UI_write_listing_window(out));  
        for (ii = 0; ii < n; ii++)  
        {  
            if (link_defs[ii].assoc_ug_tag != NULL_TAG)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(  
                    link_defs[ii].assoc_ug_tag, &type, &subtype));  
                status = UF_OBJ_ask_status(link_defs[ii].assoc_ug_tag);  
                sprintf(out, "  %d. type %d to %d (%d/%d %d) status %d\n", ii+1,  
                    link_defs[ii].link_type, link_defs[ii].assoc_ug_tag,  
                    type, subtype, status, link_defs[ii].object_status);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
            else  
            {  
                sprintf(out, "  %d. type %d to NULL_TAG\n", ii+1,  
                    link_defs[ii].link_type);  
                UF_CALL(UF_UI_write_listing_window(out));  
            }  
        }  
        if (n > 0) UF_free(link_defs);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于报告NX UDO的链接信息。
>
> 主要功能包括：
>
> 1. 打开一个列表窗口，用于显示信息。
> 2. 调用ask_udo_links函数获取UDO的链接信息，并计算链接数量。
> 3. 遍历链接数组，获取每个链接的目标对象类型、子类型、状态等，并格式化输出到列表窗口。
> 4. 如果链接数量大于0，释放链接数组占用的内存。
>
> 这段代码主要用于调试和查看UDO的链接情况，方便开发者检查UDO与外部对象的关联关系。通过输出链接的类型、目标对象的信息以及状态，可以快速了解UDO的链接情况。
>
