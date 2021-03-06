import React from 'react';
import {HiddenCard} from './HiddenCard';
import {Hand} from "./Hand";

export const OtherPlayerHand = ({ cardCount, orientation }) => {
    return <Hand cards={[...Array(cardCount)]} orientation={orientation}>
        {(card, props) => {
            return <HiddenCard {...props} />
        }}
    </Hand>;
};

