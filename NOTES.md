# 改写说明
- 所有LuceneInc.h的调用改为lucene.h
- 所有的类成员前缀加m_
- 函数遵从create_sync这样的命名法，变量遵循lockCount这样的命名法
