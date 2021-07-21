/**
 * @file Visualizer.h
 * @author Giulio Romualdi, Nicola Piga
 * @copyright Released under the terms of the MIT license
 * @date 2021
 */

#ifndef MESHCAT_CPP_VISUALIZER_H
#define MESHCAT_CPP_VISUALIZER_H

#include <Eigen/Dense>

#include <vector>
#include <string>
#include <memory>

namespace MeshcatCpp
{
    class Visualizer;
}

class MeshcatCpp::Visualizer
{
public:

    Visualizer();
    ~Visualizer();

    void display(const Eigen::Ref<const Eigen::Vector3d>& position,
                 const Eigen::Ref<const Eigen::Matrix3d>& rotation,
                 const Eigen::Ref<const Eigen::VectorXd>& jointValue,
                 const std::string& modelName = "iDyntree");

    void open();

    void setModelFromFile(const std::string& modelPath,
                          const std::vector<std::string>& jointList,
                          const std::string& modelName = "iDyntree");

    void loadModel(const std::vector<double>& color, const std::string& modelName = "iDyntree");
    bool isValid() const;
    void quit();

private:

    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

#endif // MESHCAT_CPP_VISUALIZER_H
