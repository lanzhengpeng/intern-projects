### 【2017】report c sunk hole parms 报告沉孔参数

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_c_sunk_hole_parms(tag_t c_sunk)  
    {  
        int  
            ii,  
            n_exps,  
            thru;  
        char  
            *csink_angle,  
            *depth1,  
            **descs,  
            *diameter1,  
            *diameter2,  
            *string,  
            *tip_angle;  
        tag_t  
            *exps;  
        WRITE_D(c_sunk);  
        WRITE("\nUsing UF_MODL_ask_c_sunk_hole_parms:\n");  
        UF_CALL(UF_MODL_ask_c_sunk_hole_parms(c_sunk, 0, &diameter1, &diameter2,  
            &depth1, &csink_angle, &tip_angle, &thru));  
        WRITE_S(diameter1);  
        WRITE_S(diameter2);  
        WRITE_S(depth1);  
        WRITE_S(csink_angle);  
        WRITE_S(tip_angle);  
        WRITE_L(thru);  
        UF_free(diameter1);  
        UF_free(diameter2);  
        UF_free(depth1);  
        UF_free(csink_angle);  
        UF_free(tip_angle);  
        WRITE("\nUsing UF_MODL_ask_exp_desc_of_feat:\n");  
        UF_CALL(UF_MODL_ask_exp_desc_of_feat(c_sunk, &n_exps, &descs, &exps));  
        WRITE_D(n_exps);  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(exps[ii]);  
            WRITE_S(descs[ii]);  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
            WRITE_S(string);  
            UF_free(string);  
        }  
        UF_free(exps);  
        UF_free_string_array(n_exps, descs);  
    }

```

#### 代码解析

> 这段代码是一个用于获取和报告NX中沉头孔参数的二次开发代码。主要功能包括：
>
> 1. 使用宏定义WRITE和WRITE_XXX来简化报告信息的输出。
> 2. write_integer_to_listing_window、write_string_to_listing_window、write_logical_to_listing_window函数用于将整型、字符串、逻辑值信息输出到列表窗口。
> 3. report_c_sunk_hole_parms函数用于报告沉头孔的参数信息。
> 4. 通过调用UF_MODL_ask_c_sunk_hole_parms获取沉头孔的参数信息，包括直径、深度、角度等，并输出。
> 5. 通过调用UF_MODL_ask_exp_desc_of_feat获取沉头孔特征相关的表达式描述信息，并输出。
> 6. 释放相关内存。
> 7. 使用了UF_CALL宏来简化UF函数的调用。
> 8. 使用了宏定义来简化代码，并使代码更易读。
>
> 总的来说，这段代码通过UF函数获取NX沉头孔的参数信息，并通过宏定义简化了输出报告的过程，实现了沉头孔参数的二次开发获取和报告功能。
>
