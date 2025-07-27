### 【2007】report body faces 报告体面

#### 代码

```cpp
    static void report_body_faces(tag_t body)  
    {  
        int  
            ii,  
            n_faces;  
        tag_t  
            *faces;  
        char  
            buf[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        n_faces = ask_body_faces(body, &faces);  
        sprintf(buf, "solid %d has %d faces:\n", body, n_faces);  
        UF_CALL(UF_UI_write_listing_window(buf));  
        for (ii = 0; ii < n_faces; ii++)  
        {  
            sprintf(buf, "  %d.  %d\n", ii+1, faces[ii]);  
            UF_CALL(UF_UI_write_listing_window(buf));  
        }  
        if (n_faces > 0) UF_free(faces);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要用于报告NX实体模型中的面信息。以下是代码的详细说明：
>
> 1. 定义了一个名为report_body_faces的静态函数，用于报告指定实体的面信息。
> 2. 定义了一些变量：ii用于循环计数，n_faces用于存储面的数量，faces用于存储面的标签数组，buf用于格式化输出。
> 3. 调用UF_UI_open_listing_window函数打开NX的输出窗口。
> 4. 调用ask_body_faces函数获取指定实体的面信息，并将面的标签存储在faces数组中。
> 5. 使用sprintf函数格式化输出实体编号和面的数量，并写入输出窗口。
> 6. 使用循环遍历每个面，并格式化输出面的编号，然后写入输出窗口。
> 7. 如果面的数量大于0，释放faces数组占用的内存。
> 8. 结束函数。
>
> 该函数通过获取指定实体的面信息，并在NX的输出窗口中报告每个面的编号，以便于查看实体的面组成情况。通过二次开发，可以扩展NX的功能，提高建模效率。
>
