# Retrieval Augmented Generation using LLama.cpp

Code example that uses llama.cpp to generate embeddings from a .txt file and uses the Eigen library to handle and manipulate the embedding vectors.

This assumes:

* You have already built llama.cpp and have a model (ggml or gguf format).
* You have installed Eigen.
* You are using a recent version of llama.cpp and a model with embedding support (GGUF compatible).

## Requeriments
The Llama-client requires a working install of Llama.cpp as well as Eigen library. 

`sudo apt install libcurl4-openssl-dev`

`sudo apt install libjsoncpp-dev`

`sudo apt install libeigen3-dev`

## Llama.cpp install
In order to install Llama.cpp 

`git clone https://github.com/ggml-org/llama.cpp`

`cd llama.cpp`

`cmake -B build`

 `cmake --build build --config Release`

## Clone KD-Tree RAG

`git clone https://github.com/shernandez-ucm/kdtree-eigen.git`

`git clone https://github.com/shernandez-ucm/kdtree-rag.git`

## Compile KD-Tree RAG

`cd kdtree-rag`

`cmake ./`

`make`

## Test RAG
Use Llama.cpp server to generate embeddings.

`cd ~/llama.cpp/build/bin/`

`./llama-server -m ~/kdtree-rag/models/all-MiniLM-L6-v2.Q8_0.gguf`

Use Llama_client on the cat-facts dataset.

`cd ~/kdtree-rag`

`./llama_client data/cat-facts.txt`
