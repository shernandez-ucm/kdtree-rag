# KD-Tree Retrieval Augmented Generation

Code example that uses llama.cpp to generate embeddings from a .txt file and uses the Eigen library to handle and manipulate the embedding vectors.

This assumes:

* You have already built llama.cpp and have a model (ggml or gguf format).
* You have installed Eigen.
* You are using a recent version of llama.cpp and a model withembedding support (GGUF compatible).

## Llama.cpp install

´
git clone https://github.com/ggml-org/llama.cpp
cd llama.cpp
`
