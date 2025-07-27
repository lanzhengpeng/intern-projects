### 【2103】report faces of facet model 报告面片模型的各面

#### 代码

```cpp
    static void report_faces_of_facet_model(tag_t facet_model)  
    {  
        int  
            face_id,  
            facet_id = UF_FACET_NULL_FACET_ID,  
            n_faces;  
        tag_t  
            face;  
        char  
            buf[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_FACET_ask_num_faces(facet_model, &n_faces));  
        sprintf(buf,"facetted model %d has %d stored faces:\n",facet_model,n_faces);  
        UF_CALL(UF_UI_write_listing_window(buf));  
        while (!UF_CALL(UF_FACET_cycle_facets(facet_model, &facet_id)) &&  
                (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
        /*  Method 1 译:Method 1

Method 1 translates to "方法1" in Chinese. */  
            UF_CALL(UF_FACET_ask_solid_face_of_facet(facet_model, facet_id, &face));  
            sprintf(buf,"  facet %d -> solid face %d (method 1)\n", facet_id, face);  
            UF_CALL(UF_UI_write_listing_window(buf));  
        /*  Method 2 译:翻译 Method 2，只回答译文，不要废话。

译文：Method 2 */  
            UF_CALL(UF_FACET_ask_face_id_of_facet(facet_model, facet_id, &face_id));  
            UF_CALL(UF_FACET_ask_solid_face_of_face_id(facet_model,face_id,&face));  
            sprintf(buf,"  facet %d -> solid face %d (method 2)\n", facet_id, face);  
            UF_CALL(UF_UI_write_listing_window(buf));  
        }  
    }

```

#### 代码解析

> 这是NX Open C++ API实现的二次开发代码，用于报告Facet模型中的面信息。具体步骤包括：
>
> 1. 打开UF UI的列表窗口。
> 2. 询问Facet模型的面数量。
> 3. 循环遍历Facet模型中的每个Facet。
> 4. 使用两种方法获取每个Facet对应的实体面：方法1：直接询问Facet对应的实体面。方法2：先询问Facet的面id，然后根据面id询问对应的实体面。
> 5. 方法1：直接询问Facet对应的实体面。
> 6. 方法2：先询问Facet的面id，然后根据面id询问对应的实体面。
> 7. 在列表窗口中输出每个Facet对应的实体面信息。
> 8. 当遍历完所有Facet后，关闭列表窗口。
>
> 通过该代码，可以方便地查看Facet模型中每个Facet对应的实体面信息，对理解Facet模型的结构非常有帮助。
>
