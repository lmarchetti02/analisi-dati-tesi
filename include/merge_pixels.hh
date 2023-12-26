#pragma once

#include <map>

#include "TTree.h"
#include "TFile.h"

#include "data.hh"

namespace merge_pixel
{

    void convert(std::map<Int_t, Double_t> map, std::vector<Int_t> &v_keys, std::vector<Double_t> &v_values)
    {
        for (auto &[key, value] : map)
        {
            v_keys.push_back(key);
            v_values.push_back(value);
        }
    }

    data::Entry merge(data::Entry entry, int n_pixel, int n_subpixel)
    {
        std::vector<Int_t> m_id_pixel;
        std::vector<Double_t> m_pixel_energy;
        std::vector<Int_t> m_id_pixel_cs;
        std::vector<Double_t> m_pixel_energy_cs;

        std::map<Int_t, Double_t> no_cs{};
        std::map<Int_t, Double_t> with_cs{};

        int ratio = n_subpixel / n_pixel;

        for (int i = 0; i < entry.id_pixel.size(); i++)
        {
            int id_y = entry.id_pixel[i] / n_subpixel;
            int id_x = entry.id_pixel[i] - id_y * n_subpixel;
            id_x /= ratio;
            id_y /= ratio;
            int id_new = id_y * n_pixel + id_x;

            if (no_cs.find(id_new) == no_cs.end())
                no_cs.insert(std::make_pair(id_new, entry.pixel_energy[i]));
            else
                no_cs[id_new] += entry.pixel_energy[i];
        }

        for (int i = 0; i < entry.id_pixel_cs.size(); i++)
        {
            int id_y = entry.id_pixel_cs[i] / n_subpixel;
            int id_x = entry.id_pixel_cs[i] - id_y * n_subpixel;
            id_x /= ratio;
            id_y /= ratio;
            int id_new = id_y * n_pixel + id_x;

            if (with_cs.find(id_new) == with_cs.end())
                with_cs.insert(std::make_pair(id_new, entry.pixel_energy_cs[i]));
            else
                with_cs[id_new] += entry.pixel_energy_cs[i];
        }

        merge_pixel::convert(no_cs, m_id_pixel, m_pixel_energy);
        merge_pixel::convert(with_cs, m_id_pixel_cs, m_pixel_energy_cs);

        return {entry.event_id, m_id_pixel, m_pixel_energy, m_id_pixel_cs, m_pixel_energy_cs};
    }
} // namespace merge_pixel