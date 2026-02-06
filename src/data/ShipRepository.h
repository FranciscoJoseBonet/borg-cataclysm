#pragma once
#include <vector>
#include "ShipData.h"

class ShipRepository
{
public:
    static const std::vector<ShipData> &getAllShips()
    {
        static std::vector<ShipData> ships = {

            // Enterprise
            {
                "USS Enterprise",
                "../assets/img/enterprise-001.PNG",
                100.f, // Max Health
                150.f, // Speed

                // Arma Primaria
                {"Phaser Bank", 0.2f, 600.f, 10, "../assets/img/Federation_Shot_1.png"},

                // Arma Secundaria
                {"Photon Torpedo", 1.0f, 100.f, 50, "../assets/img/Federation_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/SS_Enterprise_destruction.png",
                {128, 128},
                16,
                "La insignia de la Flota Estelar. Una nave equilibrada con buena velocidad y potencia de fuego, ideal para cualquier tipo de mision.",
                "../assets/img/Planet_Earth.png"},

            // Vulcan Vessel
            {
                "Vulcan Science Vessel",
                "../assets/img/Vulcan_Vessel.png",
                140.f,
                120.f,

                {"Plasma Array", 0.25f, 650.f, 15, "../assets/img/Vulcan_Shot_1.png"},

                {"Gravimetric Torpedo", 1.5f, 80.f, 70, "../assets/img/Vulcan_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/SS_Vulcan_Vessel_destruction.png",
                {128, 128},
                14,
                "Nave cientifica de alta resistencia. Sacrifica velocidad por escudos mejorados y armamento de plasma pesado.",
                "../assets/img/Planet_Vulcan.png"},
        };
        return ships;
    }

    static const ShipData &getShip(int index)
    {
        const auto &ships = getAllShips();
        if (index < 0 || index >= ships.size())
            return ships[0];
        return ships[index];
    }
};