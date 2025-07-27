### 【1516】list block parms 列出块参数

#### 代码

```cpp
    static void list_block_parms(tag_t feat)  
    {  
        char  
            *size[3],  
            msg[132];  
        UF_CALL(UF_MODL_ask_block_parms(feat, 1, size));  
        sprintf(msg, "\t%s, %s, %s\n", size[0], size[1], size[2]);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }

```

#### 代码解析

> 这段代码是NX Open API的C++代码，用于查询和打印NX特征（Feature）的块参数。主要功能如下：
>
> 1. 定义了一个名为list_block_parms的静态函数，接收一个NX特征类型参数。
> 2. 在函数内部，定义了字符串数组size和字符数组msg，用于存储特征参数和输出信息。
> 3. 调用UF_MODL_ask_block_parms函数，查询传入的特征参数，并将结果存储在size数组中。
> 4. 使用sprintf函数格式化字符串，将size数组中的参数拼接成输出信息，并存入msg数组。
> 5. 调用UF_UI_write_listing_window函数，在NX的列表窗口中输出msg数组中的信息。
> 6. 整个函数以static关键字声明，表示函数的作用域为文件内。
> 7. 使用UF_CALL宏调用NX Open API函数，该宏会自动处理函数返回值，在出错时打印错误信息。
> 8. 该函数的作用是查询NX特征的块参数，并以格式化的字符串形式输出到NX的列表窗口中，方便用户查看特征的具体参数。
>
