#pragma once

#include <string>
#include <iostream>

#include <Mesh.h>
#include <MeshFactory.h>
#include <Misc/Environment.h>

#include <MeshUtils/DuplicatedVertexRemoval.h>

class DuplicatedVertexRemovalTest : public ::testing::Test {
    protected:
        typedef Mesh::Ptr MeshPtr;

        virtual void SetUp() {
            std::string project_dir = Environment::get("PYMESH_PATH");
            m_data_dir = project_dir + "/tests/data/";
        }

        MeshPtr load_mesh(const std::string& filename) {
            std::string mesh_file = m_data_dir + filename;
            return MeshPtr(MeshFactory()
                    .load_file(mesh_file)
                    .create());
        }

        void assert_vector_eq(const VectorF& v1, const VectorF& v2) {
            VectorF diff = v1 - v2;
            ASSERT_FLOAT_EQ(0.0, diff.minCoeff());
            ASSERT_FLOAT_EQ(0.0, diff.maxCoeff());
        }

        void assert_matrix_eq(const MatrixFr& m1, const MatrixFr& m2) {
            MatrixFr diff = m1 - m2;
            ASSERT_FLOAT_EQ(0.0, diff.minCoeff());
            ASSERT_FLOAT_EQ(0.0, diff.maxCoeff());
        }

        void assert_matrix_eq(const MatrixIr& m1, const MatrixIr& m2) {
            MatrixIr diff = m1 - m2;
            ASSERT_EQ(0, diff.minCoeff());
            ASSERT_EQ(0, diff.maxCoeff());
        }

    protected:
        std::string m_data_dir;
};

TEST_F(DuplicatedVertexRemovalTest, 2D) {
    const Float tol = 1e-3;
    MeshPtr mesh = load_mesh("square_2D.obj");
    const size_t dim = 2;
    const size_t num_vertices = mesh->get_num_vertices();
    const size_t num_faces = mesh->get_num_faces();
    const size_t vertex_per_face = mesh->get_vertex_per_face();
    MatrixFr vertices = Eigen::Map<MatrixFr>(
            mesh->get_vertices().data(), num_vertices, dim);
    MatrixIr faces = Eigen::Map<MatrixIr>(
            mesh->get_faces().data(), num_faces, vertex_per_face);

    // Move vertex 0 to coincide with vertex 1
    vertices.row(0) = vertices.row(1);

    MatrixFr expected_vertices(num_vertices-1, dim);
    MatrixIr expected_faces(num_faces, vertex_per_face);

    for (size_t i=1; i<num_vertices; i++) {
        expected_vertices.row(i-1) = vertices.row(i);
    }
    for (size_t i=0; i<num_faces; i++) {
        for (size_t j=0; j<vertex_per_face; j++) {
            expected_faces.coeffRef(i,j) = faces.coeff(i,j) - 1;
            if (expected_faces.coeff(i,j) < 0)
                expected_faces.coeffRef(i,j) = 0;
        }
    }

    DuplicatedVertexRemoval remover(vertices, faces);
    remover.run(tol);

    MatrixFr result_vertices = remover.get_vertices();
    MatrixIr result_faces = remover.get_faces();

    assert_matrix_eq(expected_vertices, result_vertices);
    assert_matrix_eq(expected_faces, result_faces);

    ASSERT_EQ(vertices.rows() - 1, result_vertices.rows());
    ASSERT_EQ(faces.rows(), result_faces.rows());
    ASSERT_LT(result_faces.maxCoeff(), result_vertices.rows());
}

TEST_F(DuplicatedVertexRemovalTest, 3D) {
    const Float tol = 1e-3;
    MeshPtr mesh = load_mesh("cube.msh");
    const size_t dim = 3;
    const size_t num_vertices = mesh->get_num_vertices();
    const size_t num_faces = mesh->get_num_faces();
    const size_t vertex_per_face = mesh->get_vertex_per_face();

    MatrixFr vertices = Eigen::Map<MatrixFr>(
            mesh->get_vertices().data(), num_vertices, dim);
    MatrixIr faces = Eigen::Map<MatrixIr>(
            mesh->get_faces().data(), num_faces, vertex_per_face);

    // Move vertex 0 to coincide with vertex 1
    vertices.row(0) = vertices.row(1);

    MatrixFr expected_vertices(num_vertices-1, dim);
    MatrixIr expected_faces(num_faces, vertex_per_face);

    for (size_t i=1; i<num_vertices; i++) {
        expected_vertices.row(i-1) = vertices.row(i);
    }
    for (size_t i=0; i<num_faces; i++) {
        for (size_t j=0; j<vertex_per_face; j++) {
            expected_faces.coeffRef(i,j) = faces.coeff(i,j) - 1;
            if (expected_faces.coeff(i,j) < 0)
                expected_faces.coeffRef(i,j) = 0;
        }
    }

    DuplicatedVertexRemoval remover(vertices, faces);
    remover.run(tol);

    MatrixFr result_vertices = remover.get_vertices();
    MatrixIr result_faces = remover.get_faces();

    assert_matrix_eq(expected_vertices, result_vertices);
    assert_matrix_eq(expected_faces, result_faces);

    ASSERT_EQ(vertices.rows() - 1, result_vertices.rows());
    ASSERT_EQ(faces.rows(), result_faces.rows());
    ASSERT_LT(result_faces.maxCoeff(), result_vertices.rows());
}