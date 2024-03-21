/**
 *  Copyright (c) 2023 by Contributors
 *  Copyright (c) 2023, GT-TDAlab (Muhammed Fatih Balin & Umit V. Catalyurek)
 * @file graphbolt/cuda_sampling_ops.h
 * @brief Available CUDA sampling operations in Graphbolt.
 */
#ifndef GRAPHBOLT_CUDA_SAMPLING_OPS_H_
#define GRAPHBOLT_CUDA_SAMPLING_OPS_H_

#include <graphbolt/fused_sampled_subgraph.h>
#include <torch/script.h>

namespace graphbolt {
namespace ops {

/**
 * @brief Sample neighboring edges of the given nodes and return the induced
 * subgraph.
 *
 * @param indptr Index pointer array of the CSC.
 * @param indices Indices array of the CSC.
 * @param nodes The nodes from which to sample neighbors. If not provided,
 * assumed to be equal to torch.arange(indptr.size(0) - 1).
 * @param fanouts The number of edges to be sampled for each node with or
 * without considering edge types.
 *   - When the length is 1, it indicates that the fanout applies to all
 * neighbors of the node as a collective, regardless of the edge type.
 *   - Otherwise, the length should equal to the number of edge types, and
 * each fanout value corresponds to a specific edge type of the node.
 * The value of each fanout should be >= 0 or = -1.
 *   - When the value is -1, all neighbors will be chosen for sampling. It is
 * equivalent to selecting all neighbors with non-zero probability when the
 * fanout is >= the number of neighbors (and replacement is set to false).
 *   - When the value is a non-negative integer, it serves as a minimum
 * threshold for selecting neighbors.
 * @param replace Boolean indicating whether the sample is preformed with or
 * without replacement. If True, a value can be selected multiple times.
 * Otherwise, each value can be selected only once.
 * @param layer Boolean indicating whether neighbors should be sampled in a
 * layer sampling fashion. Uses the LABOR-0 algorithm to increase overlap of
 * sampled edges, see arXiv:2210.13339.
 * @param return_eids Boolean indicating whether edge IDs need to be returned,
 * typically used when edge features are required.
 * @param type_per_edge A tensor representing the type of each edge, if present.
 * @param probs_or_mask An optional tensor with (unnormalized) probabilities
 * corresponding to each neighboring edge of a node. It must be
 * a 1D tensor, with the number of elements equaling the total number of edges.
 * @param random_seed The random seed for the sampler for layer=True.
 * @param seed2_contribution The contribution of the second random seed, [0, 1)
 * for layer=True.
 *
 * @return An intrusive pointer to a FusedSampledSubgraph object containing
 * the sampled graph's information.
 */
c10::intrusive_ptr<sampling::FusedSampledSubgraph> SampleNeighbors(
    torch::Tensor indptr, torch::Tensor indices,
    torch::optional<torch::Tensor> nodes, const std::vector<int64_t>& fanouts,
    bool replace, bool layer, bool return_eids,
    torch::optional<torch::Tensor> type_per_edge = torch::nullopt,
    torch::optional<torch::Tensor> probs_or_mask = torch::nullopt,
    torch::optional<torch::Tensor> random_seed = torch::nullopt,
    float seed2_contribution = .0f);

/**
 * @brief Return the subgraph induced on the inbound edges of the given nodes.
 * @param nodes Type agnostic node IDs to form the subgraph.
 *
 * @return FusedSampledSubgraph.
 */
c10::intrusive_ptr<sampling::FusedSampledSubgraph> InSubgraph(
    torch::Tensor indptr, torch::Tensor indices, torch::Tensor nodes,
    torch::optional<torch::Tensor> type_per_edge);

}  //  namespace ops
}  //  namespace graphbolt

#endif  // GRAPHBOLT_CUDA_SAMPLING_OPS_H_
