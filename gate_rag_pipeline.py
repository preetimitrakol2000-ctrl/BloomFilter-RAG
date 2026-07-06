from bloom_bridge import BloomBridge

if __name__ == "__main__":
    gate = BloomBridge()

    # Train the filter on existing unique keywords in the knowledge base corpus
    gate.train_filter_keyword("kubernetes")
    gate.train_filter_keyword("docker")

    test_queries = ["kubernetes", "mainframe"]

    print("=== BLOOMFILTER-RAG PROBABILISTIC RETRIEVAL GATE ===")
    for query in test_queries:
        if gate.evaluate_membership(query):
            print(f"[GATE PASS] Keyword '{query}' potentially exists. Routing to vector index scan.")
        else:
            print(f"[GATE BLOCK] Keyword '{query}' definitely missing. Short-circuiting request chain.")
