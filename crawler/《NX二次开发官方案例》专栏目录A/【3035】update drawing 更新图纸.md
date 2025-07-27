### 【3035】update drawing 更新图纸

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

```

#### 代码解析

> 这段NX二次开发代码用于更新图纸中的视图，其主要功能如下：
>
> 1. 首先获取图纸对象，并查询其名称。
> 2. 判断图纸是否已过期，需要更新。如果传入的参数 always 为 TRUE，则强制更新；如果为 FALSE，则只更新过期的图纸。
> 3. 如果需要更新，则获取图纸中的所有视图对象。
> 4. 遍历每个视图，判断是否已过期，如果已过期或 always 参数为 TRUE，则更新该视图。
> 5. 更新视图后，释放视图对象数组。
> 6. 返回一个逻辑值，表示是否执行了更新操作。
>
> 通过这段代码，可以实现图纸视图的批量更新功能，提高效率。
>
