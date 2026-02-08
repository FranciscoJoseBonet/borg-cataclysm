#pragma once
#include <vector>
#include "ShipData.h"

class ShipRepository
{
public:
    static const std::vector<ShipData> &getAllShips()
    {
        static std::vector<ShipData> ships = {

            // 1. USS Enterprise (Equilibrada)
            {
                "USS Enterprise",
                "../assets/img/Ships/enterprise-001.PNG",
                100.f, // Max Health
                150.f, // Speed

                // Arma Primaria (Phaser)
                {"Phaser Bank", 0.2f, 600.f, 10, "../assets/img/Shots/Laser/Federation_Shot_1.png"},

                // Arma Secundaria (Fotones)
                {"Photon Torpedo", 1.0f, 100.f, 50, "../assets/img/Shots/Missile/Federation_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/Ships/SS_Enterprise_destruction.png",
                {128, 128}, // Tamaño frame explosión
                10,         // Frames explosión
                "La insignia de la Flota Estelar. Una nave equilibrada con buena velocidad y potencia de fuego, ideal para cualquier tipo de mision.",
                "../assets/img/Planets/Planet_Earth.png"},

            // 2. Vulcan Science Vessel
            {
                "Vulcan Science Vessel",
                "../assets/img/Ships/Vulcan_Vessel.png",
                140.f,
                120.f,

                {"Plasma Array", 0.25f, 650.f, 15, "../assets/img/Shots/Laser/Vulcan_Shot_1.png"},
                {"Gravimetric Torpedo", 1.5f, 80.f, 70, "../assets/img/Shots/Missile/Vulcan_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/Ships/SS_Vulcan_Vessel_destruction.png",
                {128, 128},
                14,
                "Nave cientifica de alta resistencia. Sacrifica velocidad por escudos mejorados y armamento de plasma pesado.",
                "../assets/img/Planets/Planet_Vulcan.png"},

            // USS Defiant
            {
                "USS Defiant",
                "../assets/img/Ships/Defiant.png",
                130.f,
                180.f,

                {"Pulse Phaser", 0.15f, 750.f, 12, "../assets/img/Shots/Laser/Defiant_Shot_1.png"},
                {"Quantum Torpedo", 1.8f, 120.f, 90, "../assets/img/Shots/Missile/Defiant_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/Ships/SS_Defiant_destruction.png",
                {128, 128},
                16,
                "Escolta pesada. Pequenia, sobre-motorizada y armada hasta los dientes. Disenada especificamente para combatir a los Borg.",
                "../assets/img/Planets/Planet_DS9.png"},

            // USS Voyager
            {
                "USS Voyager",
                "../assets/img/Ships/Voyager.png",
                110.f,
                160.f,

                {"Type X Phaser", 0.2f, 600.f, 11, "../assets/img/Shots/Laser/Federation_Shot_1.png"},
                {"Tricobalt Device", 2.5f, 70.f, 120, "../assets/img/Shots/Missile/Voyager_Shot_2.png"},

                "../assets/img/Shield.png",
                "../assets/img/Ships/SS_Voyager_destruction.png",
                {115, 230},
                8,
                "Clase Intrepid. Agil y avanzada. Cuenta con sistemas bioneurales y armamento experimental del Cuadrante Delta.",
                "../assets/img/Planets/Planet_Nebulose.png"},

            {"Andorian Kumari",
             "../assets/img/Ships/Andorian_Kumari.png",
             90.f,
             190.f,

             {"Particle Cannon", 0.18f, 800.f, 14, "../assets/img/Shots/Laser/Andorian_Shot_1.png"},
             {"Wing Missiles", 0.8f, 300.f, 35, "../assets/img/Shots/Missile/Andorian_Shot_2.png"},

             "../assets/img/Shield.png",
             "../assets/img/Ships/SS_Andorian_destruction.png",
             {99, 169},
             8,
             "Crucero de batalla de la Guardia Imperial. Sacrifica proteccion por velocidad punta y una ofensiva frontal devastadora.",
             "../assets/img/Planets/Planet_Andoria.png"},
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