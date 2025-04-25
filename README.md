# Retrieval Augmented Generation using LLama.cpp

Code example that uses llama.cpp to generate embeddings from a .txt file and uses the Eigen library to handle and manipulate the embedding vectors.

This assumes:

* You have already built llama.cpp and have a model (ggml or gguf format).
* You have installed Eigen.
* You are using a recent version of llama.cpp and a model withembedding support (GGUF compatible).

## Requeriments
The Llama-client requires a working install of Llama.cpp as well as Eigen library. 

`sudo apt install libcurl4-openssl-dev`

`sudo apt install libjsoncpp-dev`

`sudo apt install libeigen3-dev`

## Llama.cpp install
In order to install Llama.cpp 

`git clone https://github.com/ggml-org/llama.cpp`

`cd llama.cpp`