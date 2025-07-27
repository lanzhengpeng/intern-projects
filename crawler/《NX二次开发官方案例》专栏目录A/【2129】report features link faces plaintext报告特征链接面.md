### 【2129】report features link faces plaintext报告特征链接面

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_features_link_faces(tag_t feature)  
    {  
        logical  
            is_hor;  
        int  
            ii,  
            n_feats;  
        tag_t  
            dir_ref,  
            *feats,  
            target_face,  
            tool_face;  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (UF_CALL(UF_MODL_ask_feat_type(feature, &type))) return;  
        UF_CALL(UF_MODL_ask_feat_name(feature, &name));  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "feature = %d %s (%s)\n", feature, name, type);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_free(name);  
        if (!strcmp(type, "UDF_INS"))  
        {  
            UF_CALL(UF_MODL_ask_all_members_of_set(feature, &feats, &n_feats));  
            for (ii = 0; ii < n_feats; ii++)  
                report_features_link_faces(feats[ii]);  
            if (n_feats > 0) UF_free(feats);  
        }  
        else if (!UF_MODL_ask_link_faces(feature, &target_face, &tool_face,  
            &dir_ref, &is_hor))  
        {  
            WRITE_D(target_face);  
            WRITE_D(tool_face);  
            WRITE_D(dir_ref);  
            WRITE_L(is_hor);  
        }  
        else  
        {  
            WRITE("Feature not linked.\n");  
        }  
        UF_free(type);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，用于报告NX特征之间的链接关系的功能。
>
> 主要思路是通过递归遍历NX特征树，并对于每个特征，检查其是否为“UDF_INS”类型。如果是，则递归遍历其所有成员特征；如果不是，则获取该特征的目标面、工具面、方向参考和是否水平等信息，并输出到列表窗口。
>
> 代码中使用了一些宏来简化代码编写：
>
> 1. WRITE(X)：打开列表窗口并写入信息X。
> 2. WRITE_L(X)：写入逻辑值X，如果为真则输出"= TRUE"，否则输出"= FALSE"。
> 3. WRITE_D(X)：写入整数X。
> 4. report_features_link_faces(feature)：递归报告特征feature的链接面信息。
>
> 主要函数包括：
>
> 1. write_logical_to_listing_window：写入逻辑值到列表窗口。
> 2. write_integer_to_listing_window：写入整数到列表窗口。
> 3. report_features_link_faces：递归报告特征之间的链接关系。
>
> 代码首先通过UF_MODL_ask_feat_type和UF_MODL_ask_feat_name获取特征类型和名称，然后根据特征类型进行不同的处理。如果特征是“UDF_INS”，则递归处理其所有成员特征；否则，获取特征的目标面、工具面等信息，并输出到列表窗口。
>
