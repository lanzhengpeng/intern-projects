### 【3036】update drawings 更新图纸

#### 代码

```cpp
    static logical update_drawing(tag_t dwg, logical always)  
    {  
        logical  
            did_something = FALSE,  
            out_of_date;  
        int  
            ii,  
            n_views;  
        tag_t  
            *views;  
        char  
            drawing[MAX_ENTITY_NAME_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN],  
            view[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_CALL(UF_OBJ_ask_name(dwg, drawing)));  
        sprintf(msg, "  %s:  ", drawing);  
        ECHO(msg);  
        UF_CALL(UF_DRAW_is_object_out_of_date(dwg, &out_of_date));  
        if (always || out_of_date)  
        {  
            ECHO("updating views\n");  
            UF_CALL(UF_DRAW_ask_views(dwg, &n_views, &views));  
            for (ii = 0; ii < n_views; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[ii], view));  
                sprintf(msg, "    %s:  ", view);  
                ECHO(msg);  
                UF_CALL(UF_DRAW_is_object_out_of_date(views[ii], &out_of_date));  
                if (always || out_of_date)  
                {  
                    did_something = TRUE;  
                    UF_CALL(UF_DRAW_update_one_view(dwg, views[ii]));  
                    ECHO("updated\n");  
                }  
                else  
                {  
                    ECHO("up to date\n");  
                }  
            }  
        }  
        else  
        {  
            ECHO("up to date\n");  
        }  
        if (n_views > 0) UF_free(views);  
        return did_something;  
    }  
    static int update_drawings(logical always)  
    {  
        int  
            update_cnt = 0,  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
            update_cnt = update_cnt + update_drawing(dwgs[ii], always);  
        if (n_dwgs > 0) UF_free(dwgs);  
        return update_cnt;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于更新图纸视图。下面是代码的主要功能介绍：
>
> 1. update_drawing函数用于更新一个图纸中的所有视图。它会遍历图纸中的所有视图，判断视图是否过期，若过期或强制更新标志为真，则调用UF_DRAW_update_one_view函数更新该视图。
> 2. update_drawings函数用于遍历当前打开的所有图纸，对每个图纸调用update_drawing函数更新视图。它会统计更新的视图数量并返回。
> 3. 代码中大量使用UF_CALL宏来调用NX API函数，并处理返回码，保证了API调用出错时能抛出异常。
> 4. 使用了NX API函数如UF_DRAW_is_object_out_of_date来判断视图是否过期，UF_DRAW_update_one_view来更新视图，UF_DRAW_ask_views来获取图纸中的所有视图，以及UF_DRAW_ask_drawings来获取当前打开的所有图纸。
> 5. 在更新视图前后，通过打印视图名称来输出更新日志。
> 6. 在释放内存时，通过判断数组大小来避免空指针释放。
>
> 总体来说，这段代码实现了遍历当前打开的每个NX图纸，并更新图纸中所有过期视图的功能，具有较好的错误处理和日志输出功能。
>
