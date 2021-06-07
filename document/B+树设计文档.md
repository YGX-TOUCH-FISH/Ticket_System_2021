### 可重复B+树设计文档

---

#### 1. 模板设计

##### BPT模板

```c++
template<typename Key, typename Data, int MAX = 200, int CACHESIZE = 500>
class BPlusTree {...}
```

##### BPT构造

**不支持**默认构造。

```c++
RA::BPlusTree<key_type, data_type> BPT;//报错
```

**仅支持**针对文件的构造。

```
RA::BPlusTree<key_type, data_type> BPT(file1, file2);//正确
```

- 注1：file1, file2为两个std::string类型变量.

- 注2：

  **file1**是BPT数据结构的存储文件，存储内部节点、叶节点等索引信息

​	   **file2**是关于value_type数据的原始存储文件，存储value_type的原始信息

- 注3：**请务必确保file1、file2文件名对应文件在调用BPT构造函数时存在！**



**模板实例化注意事项**

**key** : 支持默认构造、拷贝赋值、赋值运算、写入文件、**<比较运算符**(即可)

**data** : 支持默认构造、拷贝构造、赋值运算、写入文件

**value_type** : std::pair<key, data>

---

#### 2.接口实现

##### 查找

支持对某一key值的寻找，返回一个data_type类的vector，存有所有键值为key的data.

```c++
vector<data_type> findTrain(const key_type &key);
```

> 不存在时返回空vector.



##### 插入

```c++
void insert(const key_type &key, const data_type &data);

typedef value_type std::pair<key_type, data_type>;
void insert(const value_type &value);
```



##### 删除

```c++
bool erase(const key_type &key, const data_type &data);

typedef value_type std::pair<key_type, data_type>;
bool erase(const value_type &value);
```



##### 修改

修改<key, data>元素的data为update

```c++
bool modify(const key_type &key, const data_type &data, const data_type &update);

bool modify(const value_type &value, const data_type &update);
```



#### 3.其他

##### RA::vector

升序排序用法：

```c++
RA::vector<data_type> a;
a.sort();//对a中所有元素全排序(升序排序)
```



##### 缓存实现

- HashMap类

  **作用：工具类**

  ```c++
  template<typename Key, typename Data, typename Hash = std::hash<Key>>
  class HashMap {...}
  ```

  **散列表 + std::hash**实现一个简单的哈希表。(CAPACITY = 401)

  

- DiskMonitor类

  **作用：管理B+树类的读写（文件读写及缓存读写）**

  ```c++
  template<typename InternInfo, typename HeadInfo, int CACHESIZE = 500>
  class DiskMonitor{...}
  //InternInfo类为在文件内部读写的信息类，如BPT的节点，如std::pair<key, data>的原始数据
  //HeadInfo类为在文件头部读写的信息类
  ```

  

